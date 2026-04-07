namespace podio{
  namespace root_utils{
    struct CollectionWriteInfo {
      uint32_t collectionID{static_cast<uint32_t>(-1)}; ///< collection id
      std::string dataType{};                           ///< The fully qualified data type of the collection
      bool isSubset{false};                             ///< Whether this collection is a subset collection or not
      unsigned int schemaVersion{0};                    ///< The schema version of the collection type
      std::string name{};                               ///< The name of the collection
      std::string storageType{};                        ///< The type in which the data is actually stored
    };
  }
 
}
 
#pragma link C++ class podio::root_utils::CollectionWriteInfo+;
#pragma link C++ class vector<podio::root_utils::CollectionWriteInfo>+;
 
#include <TTree.h>
#include <TFile.h>
#include <iostream>
 
void Podio()  {
  TFile *_file0 = TFile::Open("root://dtn-eic.jlab.org//volatile/eic/EPIC/RECO/25.10.3/epic_craterlake/EXCLUSIVE/DDVCS_ABCONV/EpIC1.1.6-1.0/18x275/q2_0_10/edecay/hminus/EpIC1.1.6-1.0_DDVCS_18x275_q2_0_10_edecay_hminus_abconv_run0.0000.eicrecon.edm4eic.root");
  // 1. Get your TTree object 
  TTree* tree = _file0->Get<TTree>("podio_metadata");
 
  std::vector<podio::root_utils::CollectionWriteInfo> *info=nullptr;
  tree->SetBranchAddress("events___CollectionTypeInfo", &info);
 
  tree->GetEntry(0);
  std::cout<<info->size()<<std::endl;
  for(const auto& entry:*info ){
    std::cout<<entry.collectionID<<" "<<entry.name<<std::endl;
  }
}
