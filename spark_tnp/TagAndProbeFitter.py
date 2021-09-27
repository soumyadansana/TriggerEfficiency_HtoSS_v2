from array import array
import ROOT
import tdrstyle
ROOT.gROOT.SetBatch()
tdrstyle.setTDRStyle()


class TagAndProbeFitter:

    def __init__(self, name, resonance='Z'):
        self._name = name
        self._w = ROOT.RooWorkspace('w')
        self._useMinos = True
        self._hists = {}
        self._resonance = resonance
        if resonance == 'Z':
            self._peak = 90
            self._fit_var_min = 60
            self._fit_var_max = 140
            self._fit_range_min = 70
            self._fit_range_max = 110
        elif resonance == 'JPsi':
            self._peak = 3.10
            self._fit_var_min = 2.80
            self._fit_var_max = 3.40
            self._fit_range_min = 2.90
            self._fit_range_max = 3.30
        self.set_fit_var()
        self.set_fit_range()

    def wsimport(self, *args):
        # getattr since import is special in python
        # NB RooWorkspace clones object
        if len(args) < 2:
            # Useless RooCmdArg: https://sft.its.cern.ch/jira/browse/ROOT-6785
            args += (ROOT.RooCmdArg(), )
        return getattr(self._w, 'import')(*args)

    def set_fit_var(self, v='x', vMin=None, vMax=None,
                    unit='GeV', label='m(#mu#mu)'):
        if vMin is None:
            vMin = self._fit_var_min
        if vMax is None:
            vMax = self._fit_var_max
        self._fitVar = v
        self._fitVarMin = vMin
        self._fitVarMax = vMax
        self._w.factory('{}[{}, {}]'.format(v, vMin, vMax))
        if unit:
            self._w.var(v).setUnit(unit)
        if label:
            self._w.var(v).setPlotLabel(label)
            self._w.var(v).SetTitle(label)

    def set_fit_range(self, fMin=None, fMax=None):
        if fMin is None:
            fMin = self._fit_range_min
        if fMax is None:
            fMax = self._fit_range_max
        self._fitRangeMin = fMin
        self._fitRangeMax = fMax

    def set_histograms(self, hPass, hFail, peak=None):
        if peak is None:
            peak = self._peak
        self._hists['Pass'] = hPass.Clone()
        self._hists['Fail'] = hFail.Clone()
        self._hists['Pass'].SetDirectory(ROOT.gROOT)
        self._hists['Fail'].SetDirectory(ROOT.gROOT)
        self._nPass = hPass.Integral()
        self._nFail = hFail.Integral()
        pb = hPass.FindBin(peak)
        nb = hPass.GetNbinsX()
        window = [int(pb-0.1*nb), int(pb+0.1*nb)]
        self._nPass_central = hPass.Integral(*window)
        self._nFail_central = hFail.Integral(*window)
        dhPass = ROOT.RooDataHist(
            'hPass', 'hPass',
            ROOT.RooArgList(self._w.var(self._fitVar)), hPass)
        dhFail = ROOT.RooDataHist(
            'hFail', 'hFail',
            ROOT.RooArgList(self._w.var(self._fitVar)), hFail)
        self.wsimport(dhPass)
        self.wsimport(dhFail)

    def set_gen_shapes(self, hPass, hFail, peak=None):
        if peak is None:
            peak = self._peak
        self._hists['GenPass'] = hPass.Clone()
        self._hists['GenFail'] = hFail.Clone()
        self._hists['GenPass'].SetDirectory(ROOT.gROOT)
        self._hists['GenFail'].SetDirectory(ROOT.gROOT)
        self._nGenPass = hPass.Integral()
        self._nGenFail = hFail.Integral()
        pb = hPass.FindBin(peak)
        nb = hPass.GetNbinsX()
        window = [int(pb-0.1*nb), int(pb+0.1*nb)]
        self._nGenPass_central = hPass.Integral(*window)
        self._nGenFail_central = hFail.Integral(*window)
        dhPass = ROOT.RooDataHist(
            'hGenPass', 'hGenPass',
            ROOT.RooArgList(self._w.var(self._fitVar)), hPass)
        dhFail = ROOT.RooDataHist(
            'hGenFail', 'hGenFail',
            ROOT.RooArgList(self._w.var(self._fitVar)), hFail)
        self.wsimport(dhPass)
        self.wsimport(dhFail)

    def set_workspace(self, lines, template=True):
        for line in lines:
            self._w.factory(line)

        nSigP = 0.9*self._nPass
        nBkgP = 0.1*self._nPass
        nSigF = 0.1*self._nFail
        nBkgF = 0.9*self._nFail
        nPassHigh = 1.1*self._nPass
        nFailHigh = 1.1*self._nFail

        if template:
            self._w.factory(
                "HistPdf::sigPhysPass({}, hGenPass)".format(self._fitVar))
            self._w.factory(
                "HistPdf::sigPhysFail({}, hGenFail)".format(self._fitVar))
            self._w.factory(
                "FCONV::sigPass({}, sigPhysPass , sigResPass)".format(
                    self._fitVar))
            self._w.factory(
                "FCONV::sigFail({}, sigPhysFail , sigResFail)".format(
                    self._fitVar))
            # update initial guesses
            nSigP = self._nGenPass_central / self._nGenPass * self._nPass if self._nGenPass > 0. else 0.
            nSigF = self._nGenFail_central / self._nGenFail * self._nFail if self._nGenFail > 0. else 0.
            if nSigP < 0.5:
                nSigP = 0.9 * self._nPass
            if nSigF < 0.5:
                nSigF = 0.1 * self._nFail

        # build extended pdf
        self._w.factory("nSigP[{}, 0.5, {}]".format(nSigP, nPassHigh))
        self._w.factory("nBkgP[{}, 0.5, {}]".format(nBkgP, nPassHigh))
        self._w.factory("nSigF[{}, 0.5, {}]".format(nSigF, nFailHigh))
        self._w.factory("nBkgF[{}, 0.5, {}]".format(nBkgF, nFailHigh))
        self._w.factory("SUM::pdfPass(nSigP*sigPass, nBkgP*bkgPass)")
        self._w.factory("SUM::pdfFail(nSigF*sigFail, nBkgF*bkgFail)")

        # import the class code in case of non-standard PDFs
        self._w.importClassCode("bkgPass")
        self._w.importClassCode("bkgFail")
        self._w.importClassCode("sigPass")
        self._w.importClassCode("sigFail")

    def fit(self, outFName, mcTruth=False, template=True):

        pdfPass = self._w.pdf('pdfPass')
        pdfFail = self._w.pdf('pdfFail')

        # if we are fitting MC truth, then set background things to constant
        if mcTruth and template:
            self._w.var('nBkgP').setVal(0)
            self._w.var('nBkgP').setConstant()
            self._w.var('nBkgF').setVal(0)
            self._w.var('nBkgF').setConstant()

        # set the range on the fit var
        # needs to be smaller than the histogram range for the convolution
        self._w.var(self._fitVar).setRange(
            self._fitRangeMin, self._fitRangeMax)
        self._w.var(self._fitVar).setRange(
            'fitRange', self._fitRangeMin, self._fitRangeMax)

        # fit passing histogram
        resPass = pdfPass.fitTo(self._w.data("hPass"),
                                ROOT.RooFit.Minos(self._useMinos),
                                ROOT.RooFit.SumW2Error(True),
                                ROOT.RooFit.Save(),
                                ROOT.RooFit.Range("fitRange"),
                                )

        # when convolving, set fail sigma to fitted pass sigma
        if template:
            self._w.var('sigmaF').setVal(
                self._w.var('sigmaP').getVal())
            self._w.var('sigmaF').setRange(
                0.8 * self._w.var('sigmaP').getVal(),
                3.0 * self._w.var('sigmaP').getVal())

        # fit failing histogram
        resFail = pdfFail.fitTo(self._w.data("hFail"),
                                ROOT.RooFit.Minos(self._useMinos),
                                ROOT.RooFit.SumW2Error(True),
                                ROOT.RooFit.Save(),
                                ROOT.RooFit.Range("fitRange"),
                                )

        # plot
        # need to run chi2 after plotting full pdf

        # pass
        pFrame = self._w.var(self._fitVar).frame(
            self._fitRangeMin, self._fitRangeMax)
        pFrame.SetTitle('Passing probes')
        self._w.data('hPass').plotOn(pFrame)
        self._w.pdf('pdfPass').plotOn(pFrame,
                                      ROOT.RooFit.Components('bkgPass'),
                                      ROOT.RooFit.LineColor(ROOT.kBlue),
                                      ROOT.RooFit.LineStyle(ROOT.kDashed),
                                      )
        self._w.pdf('pdfPass').plotOn(pFrame,
                                      ROOT.RooFit.LineColor(ROOT.kRed),
                                      )
        # -2 for the extened PDF norm for bkg and sig
        ndofp = resPass.floatParsFinal().getSize() - 2
        chi2p = pFrame.chiSquare(ndofp)
        self._w.data('hPass').plotOn(pFrame)

        # residuals/pull
        pullP = pFrame.pullHist()
        pFrame2 = self._w.var(self._fitVar).frame(
            self._fitRangeMin, self._fitRangeMax)
        pFrame2.addPlotable(pullP, 'P')

        # fail
        fFrame = self._w.var(self._fitVar).frame(
            self._fitRangeMin, self._fitRangeMax)
        fFrame.SetTitle('Failing probes')
        self._w.data('hFail').plotOn(fFrame)
        self._w.pdf('pdfFail').plotOn(fFrame,
                                      ROOT.RooFit.Components('bkgFail'),
                                      ROOT.RooFit.LineColor(ROOT.kBlue),
                                      ROOT.RooFit.LineStyle(ROOT.kDashed),
                                      )
        self._w.pdf('pdfFail').plotOn(fFrame,
                                      ROOT.RooFit.LineColor(ROOT.kRed),
                                      )
        # -2 for the extened PDF norm for bkg and sig
        ndoff = resFail.floatParsFinal().getSize() - 2
        chi2f = fFrame.chiSquare(ndoff)
        self._w.data('hFail').plotOn(fFrame)

        # residuals/pull
        pullF = fFrame.pullHist()
        fFrame2 = self._w.var(self._fitVar).frame(
            self._fitRangeMin, self._fitRangeMax)
        fFrame2.addPlotable(pullF, 'P')

        # gof tests
        statTests = ROOT.TTree('statTests', 'statTests')
        branches = {}
        branches['chi2P'] = array('f', [0])
        branches['chi2F'] = array('f', [0])
        branches['ksP'] = array('f', [0])
        branches['ksF'] = array('f', [0])
        for b in branches:
            statTests.Branch(b, branches[b], '{}/F'.format(b))

        # chi2
        branches['chi2P'][0] = chi2p
        branches['chi2F'][0] = chi2f

        # KS
        binWidth = self._hists['Pass'].GetBinWidth(1)
        nbins = int((self._fitRangeMax - self._fitRangeMin) / binWidth)
        hPdfPass = self._w.pdf('pdfPass').createHistogram(
            'ks_pdfPass',
            self._w.var(self._fitVar),
            ROOT.RooFit.Binning(nbins),
        )
        hDataPass = self._w.data('hPass').createHistogram(
            'ks_hPass',
            self._w.var(self._fitVar),
            ROOT.RooFit.Binning(nbins),
        )
        ksP = hDataPass.KolmogorovTest(hPdfPass)
        branches['ksP'][0] = ksP

        hPdfFail = self._w.pdf('pdfFail').createHistogram(
            'ks_pdfFail',
            self._w.var(self._fitVar),
            ROOT.RooFit.Binning(nbins),
        )
        hDataFail = self._w.data('hFail').createHistogram(
            'ks_hFail',
            self._w.var(self._fitVar),
            ROOT.RooFit.Binning(nbins),
        )
        ksF = hDataFail.KolmogorovTest(hPdfFail)
        branches['ksF'][0] = ksF

        statTests.Fill()

        # make canvas
        canvas = ROOT.TCanvas('c', 'c', 1100*2, 450*2)
        canvas.Divide(3, 1)

        # print parameters
        canvas.cd(1)
        eff = -1
        e_eff = 0

        nSigP = self._w.var("nSigP")
        nSigF = self._w.var("nSigF")

        nP = nSigP.getVal()
        e_nP = nSigP.getError()
        rele_nP = e_nP / nP if nP > 0. else 0.
        nF = nSigF.getVal()
        e_nF = nSigF.getError()
        rele_nF = e_nF / nF if nF > 0. else 0.
        nTot = nP + nF
        eff = nP / (nP + nF)
        e_eff = 1.0 / nTot * (rele_nP**2 + rele_nF**2)**0.5

        text1 = ROOT.TPaveText(0, 0.8, 1, 1)
        text1.SetFillColor(0)
        text1.SetBorderSize(0)
        text1.SetTextAlign(12)

        text1.AddText("Fit status pass: {}, fail: {}".format(
            resPass.status(), resFail.status()))
        text1.AddText("#chi^{{2}}/ndof pass: {:.3f}, fail: {:.3f}".format(
            chi2p, chi2f))
        text1.AddText("KS pass: {:.3f}, fail: {:.3f}".format(ksP, ksF))
        text1.AddText("eff = {:.4f} #pm {:.4f}".format(eff, e_eff))

        text = ROOT.TPaveText(0, 0, 1, 0.8)
        text.SetFillColor(0)
        text.SetBorderSize(0)
        text.SetTextAlign(12)
        text.AddText("    --- parameters ")

        def argsetToList(argset):
            arglist = []
            if not argset:
                return arglist
            argiter = argset.createIterator()
            ax = argiter.Next()
            while ax:
                arglist += [ax]
                ax = argiter.Next()
            return arglist

        text.AddText("    pass")
        listParFinalP = argsetToList(resPass.floatParsFinal())
        for p in listParFinalP:
            pName = p.GetName()
            pVar = self._w.var(pName)
            text.AddText("    - {} \t= {:.3f} #pm {:.3f}".format(
                pName, pVar.getVal(), pVar.getError()))

        text.AddText("    fail")
        listParFinalF = argsetToList(resFail.floatParsFinal())
        for p in listParFinalF:
            pName = p.GetName()
            pVar = self._w.var(pName)
            text.AddText("    - {} \t= {:.3f} #pm {:.3f}".format(
                pName, pVar.getVal(), pVar.getError()))

        text1.Draw()
        text.Draw()

        # print fit frames
        canvas.cd(2)
        plotpadP = ROOT.TPad("plotpadP", "top pad", 0.0, 0.21, 1.0, 1.0)
        ROOT.SetOwnership(plotpadP, False)
        plotpadP.SetBottomMargin(0.00)
        plotpadP.SetRightMargin(0.04)
        plotpadP.SetLeftMargin(0.16)
        plotpadP.Draw()
        ratiopadP = ROOT.TPad("ratiopadP", "bottom pad", 0.0, 0.0, 1.0, 0.21)
        ROOT.SetOwnership(ratiopadP, False)
        ratiopadP.SetTopMargin(0.00)
        ratiopadP.SetRightMargin(0.04)
        ratiopadP.SetBottomMargin(0.5)
        ratiopadP.SetLeftMargin(0.16)
        ratiopadP.SetTickx(1)
        ratiopadP.SetTicky(1)
        ratiopadP.Draw()
        if plotpadP != ROOT.TVirtualPad.Pad():
            plotpadP.cd()
        pFrame.Draw()
        ratiopadP.cd()
        pFrame2.Draw()
        prims = ratiopadP.GetListOfPrimitives()
        for prim in prims:
            if 'frame' in prim.GetName():
                prim.GetXaxis().SetLabelSize(0.19)
                prim.GetXaxis().SetTitleSize(0.21)
                prim.GetXaxis().SetTitleOffset(1.0)
                prim.GetXaxis().SetLabelOffset(0.03)
                prim.GetYaxis().SetLabelSize(0.19)
                prim.GetYaxis().SetLabelOffset(0.006)
                prim.GetYaxis().SetTitleSize(0.21)
                prim.GetYaxis().SetTitleOffset(0.35)
                prim.GetYaxis().SetNdivisions(503)
                prim.GetYaxis().SetTitle('Pull')
                prim.GetYaxis().SetRangeUser(-3, 3)
                break

        canvas.cd(3)
        plotpadF = ROOT.TPad("plotpadF", "top pad", 0.0, 0.21, 1.0, 1.0)
        ROOT.SetOwnership(plotpadF, False)
        plotpadF.SetBottomMargin(0.00)
        plotpadF.SetRightMargin(0.04)
        plotpadF.SetLeftMargin(0.16)
        plotpadF.Draw()
        ratiopadF = ROOT.TPad("ratiopadF", "bottom pad", 0.0, 0.0, 1.0, 0.21)
        ROOT.SetOwnership(ratiopadF, False)
        ratiopadF.SetTopMargin(0.00)
        ratiopadF.SetRightMargin(0.04)
        ratiopadF.SetBottomMargin(0.5)
        ratiopadF.SetLeftMargin(0.16)
        ratiopadF.SetTickx(1)
        ratiopadF.SetTicky(1)
        ratiopadF.Draw()
        if plotpadF != ROOT.TVirtualPad.Pad():
            plotpadF.cd()
        fFrame.Draw()
        ratiopadF.cd()
        fFrame2.Draw()
        prims = ratiopadF.GetListOfPrimitives()
        for prim in prims:
            if 'frame' in prim.GetName():
                prim.GetXaxis().SetLabelSize(0.19)
                prim.GetXaxis().SetTitleSize(0.21)
                prim.GetXaxis().SetTitleOffset(1.0)
                prim.GetXaxis().SetLabelOffset(0.03)
                prim.GetYaxis().SetLabelSize(0.19)
                prim.GetYaxis().SetLabelOffset(0.006)
                prim.GetYaxis().SetTitleSize(0.21)
                prim.GetYaxis().SetTitleOffset(0.35)
                prim.GetYaxis().SetNdivisions(503)
                prim.GetYaxis().SetTitle('Pull')
                prim.GetYaxis().SetRangeUser(-3, 3)
                break

        # save
        out = ROOT.TFile.Open(outFName, 'RECREATE')
        # workspace is not readable due to RooCMSShape
        # for now, just don't write
        # self._w.Write('{}_workspace'.format(self._name),
        #              ROOT.TObject.kOverwrite)
        canvas.Write('{}_Canv'.format(self._name), ROOT.TObject.kOverwrite)
        resPass.Write('{}_resP'.format(self._name), ROOT.TObject.kOverwrite)
        resFail.Write('{}_resF'.format(self._name), ROOT.TObject.kOverwrite)
        statTests.Write('{}_statTests'.format(self._name),
                        ROOT.TObject.kOverwrite)
        for hKey in self._hists:
            self._hists[hKey].Write('{}_{}'.format(self._name, hKey),
                                    ROOT.TObject.kOverwrite)
        out.Close()
        canvas.Print(outFName.replace('.root', '.png'))
