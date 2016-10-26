import FWCore.ParameterSet.Config as cms

process = cms.Process("ELEMAP")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
    fileNames = cms.untracked.vstring(
        '/store/mc/RunIIFall15DR76/DYToEE_13TeV-amcatnloFXFX-pythia8/AODSIM/PU25nsData2015v1_76X_mcRun2_asymptotic_v12-v1/80000/FCDCED20-6CCE-E511-906A-02163E00F4A6.root'
    )
)

process.load("BaconProd.Utils.ElectronFootPrint_cfi")

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('myOutputFile.root')
)

  
process.p = cms.Path(process.electronFootPrint)

process.e = cms.EndPath(process.out)
