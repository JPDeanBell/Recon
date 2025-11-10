#ifndef TRADERECORD_H
#define TRADERECORD_H
#include <string>
#include <iostream>
#include <iomanip>

struct TradeRecord{
    std::string Trade_ID;
    std::string Security_ID;
    std::string Settlement_Date;
    
    double Trade_amount;
    
    TradeRecord(): Trade_amount(0.0){}
    
    TradeRecord(const std::string& id, const std::string& sec_id,
                 const std::string& date, double amount):
                 Trade_ID(id), Security_ID(sec_id),Settlement_Date(date), Trade_amount(amount){}
    };
struct ReconciliationBreak{
    std::string Key_ID;
    std::string Break_type;
    std::string Detail;
    
    ReconciliationBreak(const std::string& key, const std::string& type, const std::string& detail):
    Key_ID(key),Break_type(type),Detail(detail){}
};
#endif
