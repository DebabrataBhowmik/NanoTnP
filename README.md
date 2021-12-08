# NanoTnP
pacakges used for TnP study on nanoaod.

```
mkdir LPC_CMSDAS2022_TnP
cd LPC_CMSDAS2022_TnP
cmsrel CMSSW_10_6_4
cd CMSSW_10_6_4/src/
bash -l
git clone https://github.com/DebabrataBhowmik/NanoTnP.git -b LPC_CMSDAS2022
cd NanoTnP
sh lxplus_setup.sh
cd analysis/
./scripts/run2018.sh
```

## Quick description

taken from https://github.com/swagata87/egm_tnp_analysis

no compilation is required (this will auto compile the C++ code automatically) but you need ROOT and RooFit installed.

This can be run on a laptop, does not require cmssw environment (still requires the setup to be run)


On lxplus (do not use cmsenv from cmssw)

Package to handle analysis of tnp trees. The main tool is the python fitter

   ===> tnpEGM_fitter.py

The interface between the user and the fitter is solely done via the settings file

   ===> analysis/settings/settings.py
   	- set the flags (i.e. Working points) that can be tested
   	- set the different samples and location
	- set the fitting bins
	- set the different cuts to be used
	- set the output directory

Help message:
>    python tnpEGM_fitter.py --help 

The settings have always to be passed to the fitter
>    python tnpEGM_fitter.py settings/settings.py 

Therefore several "settings.py" files  can be setup (for different run period for instance)


## The different fitting steps
Everything will be done for a specific flag (so the settings can be the same for different flags). Hence, the flag to be used must be specified each time (named myWP in following).

**1. Create the bining.** To each bin is associated a cut that can be tuned bin by bin in the settings.py
   * After setting up the settings.py check bins 

>   python tnpEGM_fitter.py settings.py  --flag passingMVA94Xwp90isoHWWiso0p06 --checkBins
   
   * if  you need additinal cuts for some bins (cleaning cuts), tune cuts in the settings.py, then recheck. 
     Once satisfied, create the bining

>   python tnpEGM_fitter.py settings.py  --flag passingMVA94Xwp90isoHWWiso0p06 --createBins

   * CAUTION: when recreacting bins, the output directory is overwritten! So be sure to not redo that once you are at step2

**2. Create the histograms** with the different cuts... this is the longest step. Histograms will not be re-done later
   
>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --createHists

**3. Do your first round of fits.**
   * nominal fit
   
>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit
   
   * MC fit to constrain alternate signal parameters [note this is the only MC fit that makes sense]
   
>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit --mcSig --altSig

   * Alternate signal fit (using constraints from previous fits)
   
>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit  --altSig

   * Alternate background fit (using constraints from previous fits)
   
>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit  --altBkg

**4. Check fits and redo failed ones.** (there is a web index.php in the plot directory to vizualize from the web)
   * can redo a given bin using its bin number ib. 
     The bin number can be found from --checkBins, directly in the ouput dir (or web interface)

>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit --iBin ib
   
   * the initial parameters can be tuned for this particular bin in the settings.py file. 
      Once the fit is good enough, do not redo all fits, just fix next failed fit.
      One can redo any kind of fit bin by bin. For instance the MC with altSig fit (if the constraint parameters were bad in the altSig for instance)

>   python tnpEGM_fitter.py settings.py --flag passingMVA94Xwp90isoHWWiso0p06 --doFit --mcSig --altSig --iBin ib

**5. egm txt ouput file.** Once all fits are fine, put everything in the egm format txt file

>   python tnpEGM_fitter.py settings.py  --flag passingMVA94Xwp90isoHWWiso0p06 --sumUp
   

## The settings file

The settings file includes all the necessary information for a given setup of fit

**- General settings.**

    * flag: this is the Working point in the tnpTree  (pass: flagCut ; fail !flagCut). The name of the flag myWP is the one to be passed
to the fitter. One can handle complex flags with a cut string (root cut string):
> flag = { 'myWP' : myWPCutString } 

    * baseOutDir: the output directory (will be created by the fitter)

**- Sample definition.**

    * tnpTreeDir: the directory in the tnpTree (different for phoID, eleID, reco, hlt)

    * samplesDef: these are the main info
      - data: data ntuple
      - mcNom: nominal MC sample
      - mcAlt: MC for generator syst
      - tagSel: usually same as nominal MC + different base cuts: check the tag selection syst

     The sample themselves are defined in etc/inputs/tnpSampleDef.py  (the attribute nEvts, lumi are not necessary for the fit per-se and can be omitted). 
     A list of samples for ICHEP2016 from official egm production is already setup properly in the package. 
     Then in the settings.py the sample can be specified further:
     - sample.set_mctruth() : force mc truth on a MC sample
     - sample.rename('xxx') : if a sample is used 2 times (like with 2 different sets of cuts), it has to be renamed for the second use
     - sample.set_cut(cut)  : add a cut to define the sample (like a run range for data or an additional tag selection for alt tag selection syst)
     - sample.set_weight('totWeight') : name of the weight to be used for MC reweighting (totWeight in this example). Note: the tool can handle a pu Tree to reweight a MC with different PU scenario (ask for further explanations and/or settings_rwPU.py example)
 

**- Cuts.**

    * cutBase: Define here the main cut
    * additionalCuts: can be used for cleaning cuts (or put additionalCuts = None)

**- Fitting parameters.**
    
    Define in this section the init parameters for the different fit, can be tuned to improve convergence.

====================

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
