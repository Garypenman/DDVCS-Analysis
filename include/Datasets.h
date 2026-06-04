#pragma once
#include "DatasetConfig.h"


inline std::vector<DatasetConfig> GetDatasets(){

  std::vector<DatasetConfig> datasets;

  // ---- 26.02.0 ----
  datasets.emplace_back("ddvcs","edecay","hplus","18x275","26.02.0",BkgType::None);
  datasets.emplace_back("ddvcs","edecay","hminus","18x275","26.02.0",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hplus","18x275","26.02.0",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hminus","18x275","26.02.0",BkgType::None);

  datasets.emplace_back("ddvcs","edecay","hplus","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);
  datasets.emplace_back("ddvcs","edecay","hminus","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);
  datasets.emplace_back("ddvcs","mudecay","hplus","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);
  datasets.emplace_back("ddvcs","mudecay","hminus","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);

  datasets.emplace_back("jpsi","edecay","","18x275","26.02.0",BkgType::None);
  datasets.emplace_back("jpsi","mudecay","","18x275","26.02.0",BkgType::None);
  
  datasets.emplace_back("jpsi","edecay","","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);
  datasets.emplace_back("jpsi","mudecay","","18x275","26.02.0",BkgType::Exact1S_2us_GoldCt_5um);

  
  // ---- 26.03.1 ----
  datasets.emplace_back("ddvcs","edecay","hplus","18x275","26.03.1",BkgType::None);
  datasets.emplace_back("ddvcs","edecay","hminus","18x275","26.03.1",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hplus","18x275","26.03.1",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hminus","18x275","26.03.1",BkgType::None);

  datasets.emplace_back("ddvcs","edecay","hplus","10x130","26.03.1",BkgType::None);
  datasets.emplace_back("ddvcs","edecay","hminus","10x130","26.03.1",BkgType::None);

  datasets.emplace_back("jpsi","edecay","","18x275","26.03.1",BkgType::None);
  datasets.emplace_back("jpsi","mudecay","","18x275","26.03.1",BkgType::None);

  
  // ---- 26.04.1 ----
  datasets.emplace_back("ddvcs","edecay","hplus","18x275","26.04.1",BkgType::None);
  datasets.emplace_back("ddvcs","edecay","hminus","18x275","26.04.1",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hplus","18x275","26.04.1",BkgType::None);
  datasets.emplace_back("ddvcs","mudecay","hminus","18x275","26.04.1",BkgType::None);

  datasets.emplace_back("ddvcs","edecay","hplus","10x130","26.04.1",BkgType::None);
  datasets.emplace_back("ddvcs","edecay","hminus","10x130","26.04.1",BkgType::None);

  datasets.emplace_back("jpsi","edecay","","18x275","26.04.1",BkgType::None);
  datasets.emplace_back("jpsi","mudecay","","18x275","26.04.1",BkgType::None);

  
  // ---- future 9 GeV ----
  //datasets.emplace_back("ddvcs","edecay","hplus","9x130","26.05.x",BkgType::None);
  //datasets.emplace_back("ddvcs","edecay","hminus","9x130","26.05.x",BkgType::None);

  //datasets.emplace_back("ddvcs","edecay","hplus","9x275","26.05.x",BkgType::None);
  //datasets.emplace_back("ddvcs","edecay","hminus","9x275","26.05.x",BkgType::None);

  return datasets;
}
