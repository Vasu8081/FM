// #include <ui/account/modify_account.hpp>
// #include <wx/sizer.h>

// ViewAccountPanel::ViewAccountPanel(wxWindow* parent, std::shared_ptr<account> acc) : wxPanel(parent), accountData(acc) {
//     wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
//     detailsPanel = new wxPanel(this);
//     LoadAccountDetails();
    
//     transactionsList = new wxListBox(this, wxID_ANY);
//     LoadTransactions();
    
//     mainSizer->Add(detailsPanel, 1, wxEXPAND | wxALL, 5);
//     mainSizer->Add(transactionsList, 1, wxEXPAND | wxALL, 5);
    
//     SetSizer(mainSizer);
// }

// void ViewAccountPanel::LoadAccountDetails() {
//     detailsPanel->DestroyChildren();
//     wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
//     sizer->Add(new wxStaticText(detailsPanel, wxID_ANY, "Account Name:"), 0, wxALL, 5);
//     sizer->Add(new wxStaticText(detailsPanel, wxID_ANY, accountData->getName()), 0, wxALL, 5);
    
//     detailsPanel->SetSizer(sizer);
//     detailsPanel->Layout();
// }

// void ViewAccountPanel::LoadTransactions() {
//     transactionsList->Clear();
//     model_manager& manager = model_manager::getInstance();
//     auto transactions = manager.getTransactionsForAccount(accountData->getId());
//     for (const auto& txn : transactions) {
//         transactionsList->Append(txn.to_str());
//     }
// }
