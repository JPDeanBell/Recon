#include <pybind11/pybind11.h>
#include <pybind11/stl.h> //necessray for std::vector, std::string, etc


#include "TradeRecord.h"
#include "ReconciliationEngine.h"

namespace py= pybind11;

//define module for 'reconliation_engine'
// name imported in python
PYBIND11_MODULE(recon,m){
    m.doc()= "Pybind11 module for high-speed Investment Operations reconciliation.";
    
    //1. Export the tradeRecord structure
    py::class_<TradeRecord>(m,"TradeRecord")
        .def(py::init<const std::string&, const std::string&, const std::string&, double>())
        .def_readwrite("Trade_ID",&TradeRecord::Trade_ID)
        .def_readwrite("Security_ID",&TradeRecord::Security_ID)
        .def_readwrite("Settlement_Date",&TradeRecord::Settlement_Date)
        .def_readwrite("Trade_Amount",&TradeRecord::Trade_amount);
        
    //2. Export the reconciliationBreak Structure
    py::class_<ReconciliationBreak>(m,"ReconciliationBreak")
        .def(py::init<const std::string&, const std::string&, const std::string&>())
        .def_readwrite("key_ID",&ReconciliationBreak::Key_ID)
        .def_readwrite("break_type",&ReconciliationBreak::Break_type)
        .def_readwrite("Detail",&ReconciliationBreak::Detail)
        // __repr__ for clean printing in Python
        .def("__repr__",[](const ReconciliationBreak &b){
            return"<Break: " + b.Break_type+" on ID "+ b.Key_ID + ">";
            }
        );
    // 3. Export the ReconcilationEngine class and its methods
    py::class_<ReconciliationEngine>(m,"ReconciliationEngine")
    .def(py::init<>())// default constructor
    .def("load_ledger_a",&ReconciliationEngine::load_ledger_a, "loads the master ledger(ledger A) into the C++ hash map.")
    .def("reconcile",&ReconciliationEngine::reconcile,"Performs the high-speed reconsiliation against a list of records (ledger B).");
    
    }