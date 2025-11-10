import recon
import numpy as np

from generate_ledgers import create_mock_ledgers

# Number of experiments we want
num_gen=5

for i in range(num_gen):
    #Generate mock data
    ledger_size= np.random.randint(1000,100000)
    ledger_a_df, ledger_b_df= create_mock_ledgers(ledger_size)
    print(f"Generated {len(ledger_a_df)} records for Ledger A")

    ledger_a_records=[]
    
    #creation of c++ objects from the dataFrames
    for index, row  in ledger_a_df.iterrows():
        record= recon.TradeRecord(
            row['Trade_ID'], row['Security_ID'], str(row['Settlement_Date']),row['Trade_Amount']
        )
        ledger_a_records.append(record)
    
    ledger_b_records=[]

    for index, row  in ledger_b_df.iterrows():
        record= recon.TradeRecord(
            row['Trade_ID'], row['Security_ID'], str(row['Settlement_Date']),row['Trade_Amount']
        )
        ledger_b_records.append(record)

    # Instatiating and running the C++ engine

    engine= recon.ReconciliationEngine()

    #loading the master ledger into the C++ hash

    engine.load_ledger_a(ledger_a_records)

    #figuring out the breaks
    breaks= engine.reconcile(ledger_b_records)

    print(f"\nReconciliation complete.")
    print(f"Total breaks found: {len(breaks)}")
    print("---Sample break ---")
    if breaks:
        print(breaks[0])
    print(f" End of experience {i}")
    print("===============================================================")
    print("===============================================================")
    print("===============================================================")

