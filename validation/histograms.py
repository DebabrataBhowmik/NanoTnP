# Implementing histogramming

import argparse, os
import ROOT
import numpy as np
from ROOT import array
ROOT.gROOT.SetBatch(True)

ROOT.ROOT.EnableImplicitMT(6)

# Declare range of the histogram for each variables
# Each entry in the dictionary contains of the variable name as key and a tuple
# specifying the histogram layout as value. The tuple sets the number of bins,
# the lower edge and the upper edge of the histogram.
ranges = {
    "tag_Ele_pt"     : ( 50 , 0.   , 500 ),
    "probe_Ele_pt"   : ( 50 , 0.   , 500 ),
    "tag_Ele_eta"    : ( 20 , -2.5 , 2.5 ),
    "probe_Ele_eta"  : ( 20 , -2.5 , 2.5 ),
    "pair_pt"        : ( 50 , 0.   , 500 ),
    "pair_eta"       : ( 20 , -2.5 , 2.5 ),
    "pair_mass"      : ( 80 , 50   , 130 ),
    }

# Book a histogram for a specific variable
def bookHistogram(df, variable, range_, ismc):
    ##.Filter("probe_Ele_pt > 35 && abs(probe_Ele_eta) < 2.17","high pt low eta probe ele")\
    #match="tag_PromptGenLepMatch*probe_PromptGenLepMatch"
    match="mcTrue"
    return df.Define("weights", "weight*"+ match if ismc else "weight")\
             .Filter("tag_Ele_pt > 35 && abs(tag_Ele_eta) < 2.17 && tag_Ele_q*probe_Ele_q < 0","Nominal cut")\
             .Filter("passingHWW_WP==0","passing probe flag")\
             .Histo1D(ROOT.ROOT.RDF.TH1DModel(variable, variable, range_[0], range_[1], range_[2]), variable, "weights")
pass

# Write a histogram with a given name to the output ROOT file
def writeHistogram(h, name, range_):
    h.GetYaxis().SetTitle("Events / %s GeV" % ( float( (range_[2]-range_[1]) / range_[0] ) ) )
    h.SetName(name)
    h.Write()
pass

# main function will loop over the outputs from the skimming step and produces
# required histograms for the final plotting.
# perform selection on same sign and opposite sign only
def main(sample, process):
    output="./results/"
    if 'latinov5_17' in sample:
        output+='latinov5_17/'
    elif 'latinov5_18' in sample:
        output+='latinov5_18/'

    # Create output file
    if not os.path.isdir(output):
        os.system('mkdir -p %s' % output)

    tfile = ROOT.TFile(output+process+"_hist.root", "RECREATE")
    variables = ranges.keys()
    hists={}

    # Process skimmed datasets and produce histograms of variables
    print(">>> Process skimmed sample {} for process {}".format(sample, process))

    # Load skimmed dataset and apply baseline selection (if any)
    df = ROOT.ROOT.RDataFrame("fitter_tree", sample)
    
    # Book histogram
    for variable in variables: hists[variable] = bookHistogram(df, variable, ranges[variable], False if 'Run' in process else True )

    # Write histograms to output file
    for variable in variables: writeHistogram(hists[variable], "{}_{}".format(process,variable), ranges[variable])

    tfile.Close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("sample", type=str, help="Full path to skimmed sample")
    parser.add_argument("process", type=str, help="Process name")
    #parser.add_argument("output", type=str, help="Output file with histograms")
    args = parser.parse_args()
    print(" --> samples : %s" % args.sample)
    print(" --> process : %s" % args.process)
    main(args.sample, args.process)
    
