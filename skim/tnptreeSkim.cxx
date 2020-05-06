#include "interface/helper.h"
//#include "interface/config.h"
//#include "interface/cand_sequence.h"
//#include "interface/tnpPairs_sequence.h"
//#include "interface/tree_sequence.h"


/* variable to keep */
std::vector<std::string> finalVariables = {
  "mcTrue",
  "weight",
  "tag_Ele_trigMVA",
  "event_met_pfmet",
  "event_met_pfphi",
  "pair_mass",
  "Tag_pt",
  "Tag_eta",
  "Tag_phi",
  "Tag_charge",
  "Probe_pt",
  "Probe_eta",
  "Probe_phi",
  "Probe_charge",
  "Probe_sieie",
  "Probe_eInvMinusPInv",
  "Probe_dz",
  "Probe_dxy",
  "Probe_cutBased_Fall17_V1",
  "Probe_mvaFall17V1Iso_WP90",
  "Probe_pfRelIso03_all",
  "Tag_cutBased_Fall17_V1",
};

template <typename T , typename U>
auto periodFilter(T &df, std::map< U, std::vector< std::pair<U, U> > > &m_json) {

  auto isPassJSON = [&m_json](unsigned& run , unsigned& luminosityBlock)
    {
      int RUN = static_cast<int>(run);
      int LUM = static_cast<int>(luminosityBlock);
      return Helper::isRunLumiInJSON( m_json , RUN, LUM );
    };
  
  return df
    .Define("passJSON",isPassJSON, { "run" , "luminosityBlock" } )
    .Filter("passJSON == 1"," --> Filtered by Golden Json")
    ; //here
}

/*
 * Main function, skimming step of analysis
 * The function loops over the input samples,
 * and write them to new files
 */
int main(int argc, char **argv) {

  ROOT::EnableImplicitMT(10);

  if(argc != 4) {
        std::cout << "Use executable with following arguments: ./tnptreeSkim input output integrated_luminosity" << std::endl;
        return -1;
    }

    const std::string input = argv[1];
    const std::string output = argv[2];
    const std::string lumi = argv[3];

    //std::cout << ">>> Process is mc: " << mycfg.isMC << std::endl;
    std::cout << ">>> Process input: " << input << std::endl;
    std::cout << ">>> Process output: " << output << std::endl;
    //std::cout << ">>> Integrated luminosity: " << mycfg.lumi << std::endl;
    
    // Initialize time
    TStopwatch time;
    time.Start();

    // JSON file initialization
    std::map<int, std::vector<std::pair<int, int> > > m_json;
    bool isMC = (input.find("Run") != std::string::npos) ? false : true;
    if (input.find("_16") != std::string::npos){
      if (!isMC) m_json = Helper::parseJSONAsMap("./data/Certs/Cert_271036-284044_13TeV_ReReco_07Aug2017_Collisions16_JSON.txt");
    }
    else if (input.find("_17") != std::string::npos){
      if (!isMC) m_json = Helper::parseJSONAsMap("./data/Certs/Cert_294927-306462_13TeV_EOY2017ReReco_Collisions17_JSON_v1.txt");
    }
    else{
      if (!isMC) m_json = Helper::parseJSONAsMap("./data/Certs/Cert_314472-325175_13TeV_17SeptEarlyReReco2018ABC_PromptEraD_Collisions18_JSON.txt");
    }

    // filelist
    std::vector<std::string> infiles;
    std::ifstream file(input);
    std::string str;
    while (std::getline(file, str)) { infiles.push_back(str); }
    
    ROOT::RDataFrame df("Events", infiles); // maybe make an empty dataframe?

    //auto columns = df.GetColumnNames();
    //for (auto &&columns : columns){
    //  if ( ( columns.find("Tag_") != std::string::npos ) || ( columns.find("Probe_") != std::string::npos ) || ( columns.find("TnP_") != std::string::npos ) ){
    //	if (columns.find("FatJet_") != std::string::npos ) continue;
    //	finalVariables.push_back(columns);
    //  }
    //}

    //auto df1 = hltfilter( df , mycfg , m_json ); // apply HLT and json

    // json filter
    auto df0 = periodFilter( df , m_json )
    
    // Skim
    auto df1 = df0
      .Filter("abs(Tag_pdgId)!=13 && abs(Probe_pdgId)!=13"," --> Tag and Probe are electron")
      .Filter("Probe_pt>10 && abs(Probe_eta)<2.5"," --> Probe candidate skim")
      .Filter("Tag_cutBased_Fall17_V1 == 4 && Tag_pt>30 && abs(Tag_eta)<2.1 && !(abs(Tag_eta)>= 1.4442 && abs(Tag_eta)<=1.566)"," --> Tag candidate skim")
      ;

    // dataset specific
    auto df2 = df1
      .Define("mcTrue", (isMC) ? "Tag_isGenMatched*Probe_isGenMatched" : "1")
      .Define("weight" , (isMC) ? "genWeight*baseW*puWeight" : "1")
      ;
    auto df3 = (isMC) ? df2 : df2.Filter("TnP_trigger==1"," --> data is matched to HLT filter");

    // variable for low pt cut
    auto df4 = df3
      .Define("tag_Ele_trigMVA","Tag_mvaFall17V1Iso")
      .Define("event_met_pfmet","PuppiMET_pt")
      .Define("event_met_pfphi","PuppiMET_phi")
      .Define("pair_mass","TnP_mass")
      ;

    //ROOT::RDF::SaveGraph(df,"graph_"+sample+".dot");

    auto dfFinal = df4;
    auto report = dfFinal.Report();
    dfFinal.Snapshot("fitter_tree", output, finalVariables);
    time.Stop();

    report->Print();
    time.Print();
}
