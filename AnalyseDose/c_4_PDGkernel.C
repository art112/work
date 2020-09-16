void c_4_PDGkernel()
{
//=========Macro generated from canvas: c_4_PDGkernel/c_4_PDGkernel
//=========  (Tue Jan 21 21:27:34 2020) by ROOT version 6.14/00
   TCanvas *c_4_PDGkernel = new TCanvas("c_4_PDGkernel", "c_4_PDGkernel",0,25,1745,907);
   c_4_PDGkernel->Range(-9.733333,-1.158572,76.93333,0.5557135);
   c_4_PDGkernel->SetFillColor(10);
   c_4_PDGkernel->SetBorderMode(0);
   c_4_PDGkernel->SetBorderSize(2);
   c_4_PDGkernel->SetGridx();
   c_4_PDGkernel->SetGridy();
   c_4_PDGkernel->SetTickx(1);
   c_4_PDGkernel->SetTicky(1);
   c_4_PDGkernel->SetLeftMargin(0.17);
   c_4_PDGkernel->SetRightMargin(0.08);
   c_4_PDGkernel->SetTopMargin(0.12);
   c_4_PDGkernel->SetBottomMargin(0.18);
   c_4_PDGkernel->SetFrameLineWidth(2);
   c_4_PDGkernel->SetFrameBorderMode(0);
   c_4_PDGkernel->SetFrameLineWidth(2);
   c_4_PDGkernel->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("#Delta NIST - Gate Proton range in function of beam energy for Water");
   
   Double_t Graph_fx1001[14] = {
   5,
   10,
   15,
   20,
   25,
   30,
   35,
   40,
   45,
   50,
   55,
   60,
   65,
   70};
   Double_t Graph_fy1001[14] = {
   -0.01130001,
   -0.08000002,
   -0.08999995,
   -0.1099999,
   -0.2199999,
   -0.2000004,
   -0.2500002,
   -0.3399998,
   -0.3600008,
   -0.4199996,
   -0.4899992,
   -0.5799996,
   -0.6700008,
   -0.7500008};
   Double_t Graph_fex1001[14] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph_fey1001[14] = {
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1};
   TGraphErrors *gre = new TGraphErrors(14,Graph_fx1001,Graph_fy1001,Graph_fex1001,Graph_fey1001);
   gre->SetName("Graph");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);
   gre->SetLineColor(51);
   gre->SetLineWidth(2);
   gre->SetMarkerColor(51);
   gre->SetMarkerStyle(8);
   
   TH1F *Graph_Graph1001 = new TH1F("Graph_Graph1001","Graph",100,0,76.5);
   Graph_Graph1001->SetMinimum(-0.9438709);
   Graph_Graph1001->SetMaximum(0.1825701);
   Graph_Graph1001->SetDirectory(0);
   Graph_Graph1001->SetStats(0);
   Graph_Graph1001->SetLineWidth(2);
   Graph_Graph1001->SetMarkerStyle(8);
   Graph_Graph1001->GetXaxis()->SetNdivisions(506);
   Graph_Graph1001->GetXaxis()->SetLabelFont(42);
   Graph_Graph1001->GetXaxis()->SetLabelOffset(0.015);
   Graph_Graph1001->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph1001->GetXaxis()->SetTitleFont(42);
   Graph_Graph1001->GetYaxis()->SetNdivisions(506);
   Graph_Graph1001->GetYaxis()->SetLabelFont(42);
   Graph_Graph1001->GetYaxis()->SetLabelOffset(0.015);
   Graph_Graph1001->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetYaxis()->SetTitleSize(0.05);
   Graph_Graph1001->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph1001->GetYaxis()->SetTitleFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelFont(42);
   Graph_Graph1001->GetZaxis()->SetLabelOffset(0.015);
   Graph_Graph1001->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1001->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph1001->GetZaxis()->SetTitleOffset(1.1);
   Graph_Graph1001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1001);
   
   multigraph->Add(gre,"PLC PMC");
   
   Double_t Graph_fx1002[14] = {
   5,
   10,
   15,
   20,
   25,
   30,
   35,
   40,
   45,
   50,
   55,
   60,
   65,
   70};
   Double_t Graph_fy1002[14] = {
   -0.01130001,
   -0.08000002,
   -0.08999995,
   -0.1099999,
   -0.12,
   -0.1,
   -0.1499998,
   -0.14,
   -0.16,
   -0.1200004,
   -0.08999962,
   -0.18,
   -0.1700008,
   -0.1499985};
   Double_t Graph_fex1002[14] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph_fey1002[14] = {
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1};
   gre = new TGraphErrors(14,Graph_fx1002,Graph_fy1002,Graph_fex1002,Graph_fey1002);
   gre->SetName("Graph");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);
   gre->SetLineColor(67);
   gre->SetLineWidth(2);
   gre->SetMarkerColor(67);
   gre->SetMarkerStyle(8);
   
   TH1F *Graph_Graph1002 = new TH1F("Graph_Graph1002","Graph",100,0,76.5);
   Graph_Graph1002->SetMinimum(-0.31687);
   Graph_Graph1002->SetMaximum(0.12557);
   Graph_Graph1002->SetDirectory(0);
   Graph_Graph1002->SetStats(0);
   Graph_Graph1002->SetLineWidth(2);
   Graph_Graph1002->SetMarkerStyle(8);
   Graph_Graph1002->GetXaxis()->SetNdivisions(506);
   Graph_Graph1002->GetXaxis()->SetLabelFont(42);
   Graph_Graph1002->GetXaxis()->SetLabelOffset(0.015);
   Graph_Graph1002->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1002->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph1002->GetXaxis()->SetTitleFont(42);
   Graph_Graph1002->GetYaxis()->SetNdivisions(506);
   Graph_Graph1002->GetYaxis()->SetLabelFont(42);
   Graph_Graph1002->GetYaxis()->SetLabelOffset(0.015);
   Graph_Graph1002->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1002->GetYaxis()->SetTitleSize(0.05);
   Graph_Graph1002->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph1002->GetYaxis()->SetTitleFont(42);
   Graph_Graph1002->GetZaxis()->SetLabelFont(42);
   Graph_Graph1002->GetZaxis()->SetLabelOffset(0.015);
   Graph_Graph1002->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1002->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph1002->GetZaxis()->SetTitleOffset(1.1);
   Graph_Graph1002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1002);
   
   multigraph->Add(gre,"PLC PMC");
   
   Double_t Graph_fx1003[14] = {
   5,
   10,
   15,
   20,
   25,
   30,
   35,
   40,
   45,
   50,
   55,
   60,
   65,
   70};
   Double_t Graph_fy1003[14] = {
   -0.01130001,
   -0.08000002,
   -0.08999995,
   -0.01,
   -0.02000009,
   -0.1,
   -0.05000038,
   -0.03999962,
   -0.05999962,
   -0.02,
   0.01000076,
   0.02000076,
   0.03,
   0.04999847};
   Double_t Graph_fex1003[14] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph_fey1003[14] = {
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1};
   gre = new TGraphErrors(14,Graph_fx1003,Graph_fy1003,Graph_fex1003,Graph_fey1003);
   gre->SetName("Graph");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);
   gre->SetLineColor(83);
   gre->SetLineWidth(2);
   gre->SetMarkerColor(83);
   gre->SetMarkerStyle(8);
   
   TH1F *Graph_Graph1003 = new TH1F("Graph_Graph1003","Graph",100,0,76.5);
   Graph_Graph1003->SetMinimum(-0.2349999);
   Graph_Graph1003->SetMaximum(0.1849983);
   Graph_Graph1003->SetDirectory(0);
   Graph_Graph1003->SetStats(0);
   Graph_Graph1003->SetLineWidth(2);
   Graph_Graph1003->SetMarkerStyle(8);
   Graph_Graph1003->GetXaxis()->SetNdivisions(506);
   Graph_Graph1003->GetXaxis()->SetLabelFont(42);
   Graph_Graph1003->GetXaxis()->SetLabelOffset(0.015);
   Graph_Graph1003->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph1003->GetXaxis()->SetTitleFont(42);
   Graph_Graph1003->GetYaxis()->SetNdivisions(506);
   Graph_Graph1003->GetYaxis()->SetLabelFont(42);
   Graph_Graph1003->GetYaxis()->SetLabelOffset(0.015);
   Graph_Graph1003->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetYaxis()->SetTitleSize(0.05);
   Graph_Graph1003->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph1003->GetYaxis()->SetTitleFont(42);
   Graph_Graph1003->GetZaxis()->SetLabelFont(42);
   Graph_Graph1003->GetZaxis()->SetLabelOffset(0.015);
   Graph_Graph1003->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1003->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph1003->GetZaxis()->SetTitleOffset(1.1);
   Graph_Graph1003->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1003);
   
   multigraph->Add(gre,"PLC PMC");
   
   Double_t Graph_fx1004[14] = {
   5,
   10,
   15,
   20,
   25,
   30,
   35,
   40,
   45,
   50,
   55,
   60,
   65,
   70};
   Double_t Graph_fy1004[14] = {
   -0.01130001,
   -0.08000002,
   -0.08999995,
   -0.01,
   -0.02000009,
   3.814697e-07,
   0.05,
   0.05999981,
   0.04000076,
   0.08000038,
   0.1099992,
   0.1199992,
   0.2300008,
   0.2499992};
   Double_t Graph_fex1004[14] = {
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph_fey1004[14] = {
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1,
   0.1};
   gre = new TGraphErrors(14,Graph_fx1004,Graph_fy1004,Graph_fex1004,Graph_fey1004);
   gre->SetName("Graph");
   gre->SetTitle("Graph");
   gre->SetFillStyle(1000);
   gre->SetLineColor(99);
   gre->SetLineWidth(2);
   gre->SetMarkerColor(99);
   gre->SetMarkerStyle(8);
   
   TH1F *Graph_Graph1004 = new TH1F("Graph_Graph1004","Graph",100,0,76.5);
   Graph_Graph1004->SetMinimum(-0.2439999);
   Graph_Graph1004->SetMaximum(0.4039992);
   Graph_Graph1004->SetDirectory(0);
   Graph_Graph1004->SetStats(0);
   Graph_Graph1004->SetLineWidth(2);
   Graph_Graph1004->SetMarkerStyle(8);
   Graph_Graph1004->GetXaxis()->SetNdivisions(506);
   Graph_Graph1004->GetXaxis()->SetLabelFont(42);
   Graph_Graph1004->GetXaxis()->SetLabelOffset(0.015);
   Graph_Graph1004->GetXaxis()->SetLabelSize(0.05);
   Graph_Graph1004->GetXaxis()->SetTitleSize(0.05);
   Graph_Graph1004->GetXaxis()->SetTitleFont(42);
   Graph_Graph1004->GetYaxis()->SetNdivisions(506);
   Graph_Graph1004->GetYaxis()->SetLabelFont(42);
   Graph_Graph1004->GetYaxis()->SetLabelOffset(0.015);
   Graph_Graph1004->GetYaxis()->SetLabelSize(0.05);
   Graph_Graph1004->GetYaxis()->SetTitleSize(0.05);
   Graph_Graph1004->GetYaxis()->SetTitleOffset(1.1);
   Graph_Graph1004->GetYaxis()->SetTitleFont(42);
   Graph_Graph1004->GetZaxis()->SetLabelFont(42);
   Graph_Graph1004->GetZaxis()->SetLabelOffset(0.015);
   Graph_Graph1004->GetZaxis()->SetLabelSize(0.05);
   Graph_Graph1004->GetZaxis()->SetTitleSize(0.05);
   Graph_Graph1004->GetZaxis()->SetTitleOffset(1.1);
   Graph_Graph1004->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph1004);
   
   multigraph->Add(gre,"PLC PMC");
   multigraph->Draw("a PLC PMC");
   multigraph->GetXaxis()->SetTitle("Beam energy [MeV]");
   multigraph->GetXaxis()->SetMoreLogLabels();
   multigraph->GetXaxis()->SetNdivisions(1020);
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelOffset(0.015);
   multigraph->GetXaxis()->SetTitleOffset(1.3);
   multigraph->GetXaxis()->SetTitleFont(42);
   multigraph->GetYaxis()->SetTitle("Depth [mm]");
   multigraph->GetYaxis()->SetNdivisions(1020);
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelOffset(0.015);
   multigraph->GetYaxis()->SetTitleOffset(1.35);
   multigraph->GetYaxis()->SetTitleFont(42);
   
   TPaveText *pt = new TPaveText(0.01,0.9394211,0.71,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetLineWidth(2);
   TText *pt_LaTex = pt->AddText("#Delta NIST - Gate Proton range in function of beam energy for Water");
   pt->Draw();
   TLine *line = new TLine(5,0.1,70,0.1);
   line->SetLineStyle(9);
   line->SetLineWidth(4);
   line->Draw();
   line = new TLine(5,-0.1,70,-0.1);
   line->SetLineStyle(9);
   line->SetLineWidth(2);
   line->Draw();
   
   TLegend *leg = new TLegend(0.1903024,0.2312925,0.4014599,0.4501134,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetTextAlign(22);
   leg->SetTextSize(0.025);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(2);
   leg->SetFillColor(10);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("Graph","Water 67.00 eV","alp");
   entry->SetLineColor(51);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(51);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph","Water 75.00 eV","alp");
   entry->SetLineColor(67);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(67);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph","Water 78.00 eV","alp");
   entry->SetLineColor(83);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(83);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph","Water 80.80 eV","alp");
   entry->SetLineColor(99);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(99);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   c_4_PDGkernel->Modified();
   c_4_PDGkernel->cd();
   c_4_PDGkernel->SetSelected(c_4_PDGkernel);
}
