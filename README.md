# NanoTnP
pacakges used for TnP study on nanoaod.

```
mkdir CMSDAS2020_TnP
cd CMSDAS2020_TnP
cmsrel CMSSW_10_6_4
cd CMSSW_10_6_4/src/
bash -l
git clone git@github.com:arunhep/NanoTnP.git
git checkout CMSDAS2020
sh lxplus_setup.sh
cd NanoTnP/analysis/
./scripts/run2018_v1.sh
```




# Samples used for input ntuples
```
# Run2018 v7
Samples["EGamma_Run2018A-02Apr2020-v1"]      = {'nanoAOD': '/EGamma/Run2018A-02Apr2020-v1/NANOAOD'}
Samples["EGamma_Run2018B-02Apr2020-v1"]      = {'nanoAOD': '/EGamma/Run2018B-02Apr2020-v1/NANOAOD'}
Samples["EGamma_Run2018C-02Apr2020-v1"]      = {'nanoAOD': '/EGamma/Run2018C-02Apr2020-v1/NANOAOD'}
Samples["EGamma_Run2018D-02Apr2020-v1"]      = {'nanoAOD': '/EGamma/Run2018D-02Apr2020-v1/NANOAOD'}

# MC Autumm v7
Samples['DYJetsToLL_M-50'] = {'nanoAOD' :'/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21-v1/NANOAODSIM'}
Samples['DYJetsToLL_M-50_ext2'] = {'nanoAOD' :'/DYJetsToLL_M-50_TuneCP5_13TeV-amcatnloFXFX-pythia8/RunIIAutumn18NanoAODv7-Nano02Apr2020_102X_upgrade2018_realistic_v21_ext2-v1/NANOAODSIM'}

```
