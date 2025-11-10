#ifndef RECONCILIATIONENGINE_H
#define RECONCILIATIONENGINE_H
#include "TradeRecord.h"
#include <unordered_map>
#include <vector>
#include <set>
#include <cmath>

class ReconciliationEngine{
    private:
        // Ledger A: stored for fast lookup using TRade_ID as the primary key
        std::unordered_map<std::string, TradeRecord> ledger_a_map;
        
        //define a small tolerance for floating-point comparisons
        const double TOLERANCE=.001;
    public:
        double getTolerance() const{
            return TOLERANCE;
            }
        void load_ledger_a(const std::vector<TradeRecord>& records){
            ledger_a_map.clear();
            for(const auto& record: records){
                
                ledger_a_map[record.Trade_ID]=record;
            }
            std::cout << "Ledger A loaded. Totol records: " << ledger_a_map.size() << std::endl;
        }
        
        std::vector<ReconciliationBreak> reconcile(const std::vector<TradeRecord>& ledger_b_records){
            std::vector<ReconciliationBreak> breaks;
            
            //set to track IDS from ledger A that are successfully matched in ledger B
            std::set<std::string> matched_a_ids;
            
            //--------------PHASE 1: A vs B, B vs A checks -------------
            
            for(const auto& record_b: ledger_b_records){
                auto it= ledger_a_map.find(record_b.Trade_ID);
                
                if(it == ledger_a_map.end()){
                    //--- BREAK TYPE 1: MISSING IN A (record unique to B)---
                    breaks.emplace_back(record_b.Trade_ID,"UNIQUE_TO_B","Transaction exists only in ledger B");
                    continue;
                    }
                    
                //Record found in both legders
                matched_a_ids.insert(record_b.Trade_ID); //*** TRACK AS MATCHED ***
                const TradeRecord& record_a= it->second;
                
                // --- BREAK TYPE 2&3 :KEY MISMATCH & MONETARY DISCREPANCY (value Breaks) ---
            if(record_a.Security_ID != record_b.Security_ID || record_a.Settlement_Date!= record_b.Settlement_Date){
                breaks.emplace_back(record_b.Trade_ID,"KEY_MISMATCH",
                                    "Security ID or Settlement Date differs.");
                }
            if(std::abs(record_a.Trade_amount- record_b.Trade_amount)> TOLERANCE){
                std::string detail= "A: "+std::to_string(record_a.Trade_amount);
                detail+=", B: ";
                detail+= std::to_string(record_b.Trade_amount);
                
                breaks.emplace_back(record_b.Trade_ID,"AMOUNT_DISCREPANCY",detail);
                }
                }
                // ------------------- PHASE 2: Check for Records Unique to A -----------
                //Iterate through ALL records loaded in ledger A
                
                for(const auto& pair: ledger_a_map){
                    const std::string& trade_id= pair.first;
                    
                    //If the id is not found in the 'matched_a_ids' set from phase 1, it's a break
                    if(matched_a_ids.find(trade_id)==matched_a_ids.end()){
                        //--- Break Type 4: Missing in B (Record unique to A)---
                        breaks.emplace_back(trade_id, "UNIQUE_TO_A", "Transaction nooked internally (A) but never reported/settled externally (B).");
                        }
                    }
                return breaks;
            }

};
#endif