import pandas as pd
import numpy as np
from faker import Faker
import uuid
import random
from datetime import timedelta

def create_mock_ledgers(num_trades:int):
    """Generate two mock ledgers (A and B) with intentional breaks"""
    fake= Faker()
    
    #--- 1. Create ledger A (Internal Books) ---
    data_a={
        'Trade_ID':[str(uuid.uuid4()) for _ in range(num_trades)],
        'Security_ID':[fake.bothify(text='US######') for _ in range(num_trades)],
        'Trade_Date':[fake.date_between(start_date='-1y', end_date='today') for _ in range(num_trades)],
        'Quantity': np.random.randint(100,1000,num_trades),
        'Trade_Amount': np.round(np.random.uniform(5000,500000,num_trades),2),
        'Counterparty_ID':[f'CP{i}' for i in np.random.randint(1,100,num_trades)],
        'Client_Account_ID':[f'CLI{i}' for i in np.random.randint(1000,2000,num_trades)],
    }
    ledger_a= pd.DataFrame(data_a)

    #calulate Settlement Date (T+2)
    ledger_a['Settlement_Date']=ledger_a['Trade_Date']+timedelta(days=2)

    #--- 2. Create Ledger B (external Report) by copying A ---

    ledger_b= ledger_a.copy()

    #-- 3.breaks---

    #Monetary Discrepancy 10% of trades
    break_indices_1= ledger_b.sample(frac=0.1, replace=False).index
    ledger_b.loc[break_indices_1,'Trade_Amount']+=np.round(np.random.uniform(-100,100,len(break_indices_1)),2)

    #key Mismatch (5% of trades- change setllement date)
    break_indices_2= ledger_b.sample(frac=0.05, replace=False).index
    ledger_b.loc[break_indices_2,'Settlement_Date']+=timedelta(days=1)

    # Missing transactions 5% deleted from B
    rows_to_delete= ledger_b.sample(frac=.05, replace=False).index
    ledger_b=ledger_b.drop(rows_to_delete)

    #Reset indices
    ledger_a=ledger_a.reset_index(drop=True)
    ledger_b=ledger_b.reset_index(drop=True)

    return ledger_a, ledger_b



