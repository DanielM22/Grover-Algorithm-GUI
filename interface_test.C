#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TMath.h>
#include <TRootEmbeddedCanvas.h>
#include <RQ_OBJECT.h>
#include <TGraph.h>
#include "TGeoManager.h"
#include "TGeometry.h"
#include "TGTab.h"

#include <iostream>
#include <cmath>
#include <vector>

class MyMainFrame{
  RQ_OBJECT("MyMainFrame")
  private:
  TGMainFrame         *MainF;
  TRootEmbeddedCanvas *MCanvas;
  TGVerticalFrame     *VFrame12, *VFrame2;
  TGHorizontalFrame   *HFrame1, *HFrame2, *HFrame3, *VFrame1;
  TGCompositeFrame    *MainCFrame, *SimulF, *ConceptF;
  TGTab               *SimulTab, *BConceptsTab;
  TGTextButton        *TButton1, *TButton2, *TButton3, *next, *previous, *Gates, *AlSteps, *RVal;
  TGNumberEntry       *NEntry1, *NEntry2, *RValue;
  TGLabel             *Label1, *Label2, *Label3;
  TGGroupFrame        *GFrame1, *GFrame2, *GFrame3;
  TGraph              *PEstados;
  
  TLine               *Circuit1, *Circuit2, *Circuit3, *Circuit4, *Axis1, *Axis2, *SepLine;
  TLatex               latex;
  TArrow              *SVector;
  TPaveText           *HGate, *HGate2, *HGate3, *HGate4, *Steps, *UfGate, *UfGate2, *UfGate3, *UfGate4, *DifGate, *DifGate2, *DifGate3, *DifGate4;
  TColor              *GateColor = gROOT->GetColor(20);
  TCanvas             *fCanvas;
  
  double *M_state;
  int n, len, N, k;
  double ms[];
  
  
  public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  //virtual ~MyMainFrame();
  void DoDraw();
  void Step1();
  void Oracle();
  void Diffuser();
  void GateConstruct();
  void Algorithm();
  void RDefinition();
};

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   MainF = new TGMainFrame(p,w,h);
   MainF->SetWMSizeHints(w,h,1600,900,10,10);
   MainF->SetWindowName("Grover's Algorithm");


   HFrame1 = new TGHorizontalFrame(MainF);
   VFrame1 = new TGHorizontalFrame(MainF);
   HFrame2 = new TGHorizontalFrame(VFrame1);
   VFrame2 = new TGVerticalFrame(HFrame1);
   HFrame3 = new TGHorizontalFrame(VFrame1);
   MainCFrame = new TGCompositeFrame(HFrame1, kVerticalFrame);
   SimulTab = new TGTab(MainCFrame);
   BConceptsTab = new TGTab(MainCFrame);
   
   SimulF = SimulTab->AddTab("Simulation");
   SimulF->SetLayoutManager(new TGVerticalLayout(SimulF));
   ConceptF = SimulTab->AddTab("Basic Concepts");
   ConceptF->SetLayoutManager(new TGVerticalLayout(ConceptF));

   MCanvas = new TRootEmbeddedCanvas("Lienzo",HFrame1,w,h);

   //GFrame1 = new TGGroupFrame(VFrame2,"Number of Elements");
   GFrame1 = new TGGroupFrame(SimulF,"Number of Elements");
   //GFrame2 = new TGGroupFrame(VFrame2, "Search Element");
   GFrame2 = new TGGroupFrame(SimulF, "Search Element");

   TButton1 = new TGTextButton (HFrame2, "Start");
   TButton1->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   TButton2 = new TGTextButton (HFrame2, "Restart");
   TButton3 = new TGTextButton (HFrame2, "Close");
   next = new TGTextButton (HFrame2, "Next");
   previous = new TGTextButton (HFrame2, "Previous");

   Gates = new TGTextButton(ConceptF, "Gates");
   Gates->Connect("Clicked()", "MyMainFrame", this, "GateConstruct()");
   AlSteps = new TGTextButton(ConceptF, "Algorithm Steps");
   AlSteps->Connect("Clicked()", "MyMainFrame", this, "Algorithm()");
   RVal = new TGTextButton(ConceptF, "R Value");
   RVal->Connect("Clicked()", "MyMainFrame", this, "RDefinition()");
  
   //Label2 = new TGLabel(VFrame2, "Number of Elements");
   //Label1 = new TGLabel(VFrame2, "Search Element");
   NEntry1 = new TGNumberEntry(GFrame1);
   NEntry1->SetLimits(TGNumberFormat::kNELLimitMinMax,3,16);
   
   NEntry2 = new TGNumberEntry(GFrame2);
   NEntry2->SetLimits(TGNumberFormat::kNELLimitMinMax,1,16);
   
   //Label3 = new TGLabel(VFrame2, "R Value:");
   Label3 = new TGLabel(SimulF, "R Value:");

   //RValue = new TGNumberEntry(VFrame2);
   RValue = new TGNumberEntry(SimulF);
   RValue->SetState(kFALSE);

   
   TGLayoutHints *Centrar = new TGLayoutHints (kLHintsCenterX | kLHintsCenterY ,5 ,5 ,5 ,5);
   TGLayoutHints *Expandir = new TGLayoutHints (kLHintsExpandX | kLHintsExpandY ,10 ,10 ,10 ,10);
   TGLayoutHints *CentrarX = new TGLayoutHints (kLHintsCenterX ,5 ,5);
   TGLayoutHints *CentrarY = new TGLayoutHints (kLHintsCenterY ,5 ,5);
   TGLayoutHints *ExpandirX = new TGLayoutHints (kLHintsExpandX ,0 ,10);
   TGLayoutHints *ExpandirY = new TGLayoutHints (kLHintsExpandY ,10 ,10);

   TGIcon *IconUD= new TGIcon(HFrame3, "pictures/LogoUD1.png");
   TGIcon *IFisinfor = new TGIcon(HFrame3, "pictures/logo_fisinfor.png");

   /* VFrame2->AddFrame(Label2,CentrarX);
   VFrame2->AddFrame(NEntry2,CentrarX);
   VFrame2->AddFrame(Label1,Centrar);
   VFrame2->AddFrame(NEntry1,Centrar);
   VFrame2->AddFrame(Label3,CentrarX);*/

   GFrame1->AddFrame(NEntry1,Centrar);
   GFrame2->AddFrame(NEntry2,Centrar);

   /*VFrame2->AddFrame(GFrame1,CentrarX);
   VFrame2->AddFrame(GFrame2,CentrarX);
   VFrame2->AddFrame(Label3,CentrarX);
   VFrame2->AddFrame(RValue,CentrarX);*/

   SimulF->AddFrame(GFrame1,CentrarX);
   SimulF->AddFrame(GFrame2,CentrarX);
   SimulF->AddFrame(Label3,CentrarX);
   SimulF->AddFrame(RValue,CentrarX);

   ConceptF->AddFrame(Gates, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));
   ConceptF->AddFrame(AlSteps, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));     
   ConceptF->AddFrame(RVal, new TGLayoutHints(kLHintsCenterX, 20,5,5,20));

   HFrame2->AddFrame(TButton1,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX, 5,5,5,5));
   HFrame2->AddFrame(TButton2,Centrar);
   HFrame2->AddFrame(TButton3,Centrar);
   HFrame2->AddFrame(previous, Centrar);
   HFrame2->AddFrame(next, Centrar);

   
   HFrame3->AddFrame(IFisinfor, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IFisinfor->Resize(70,85);
   HFrame3->AddFrame(IconUD, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IconUD->Resize(70,80);
   

   //HFrame1->AddFrame(VFrame2,ExpandirY);
   MainCFrame->AddFrame(SimulTab,ExpandirY);
   //MainCFrame->AddFrame(BConceptsTab,ExpandirY);

   HFrame1->AddFrame(MainCFrame, ExpandirY);
   HFrame1->AddFrame(MCanvas, Expandir);
   VFrame1->AddFrame(HFrame2,new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 2,2,2,2));
   VFrame1->AddFrame(HFrame3,new TGLayoutHints(kLHintsRight, 2,2,0,0));

   MainF->AddFrame(HFrame1,Expandir);
   //MainF->AddFrame(MCanvas,Expandir);
   MainF->AddFrame(VFrame1,ExpandirX);

   
   TText *t = new TText(0.5,0.95,"SE GROVER'S ALGORITHM");
   t->SetTextAlign(22);
   t->SetTextColor(kRed+2);
   t->SetTextFont(22);
   t->SetTextSize(0.05);
   //t->SetTextAngle(45);
   t->Draw();
   

   TPaveText *PaveInfo = new TPaveText(0.01,0.01,0.42,0.9);
   PaveInfo->SetTextAlign(13);
   PaveInfo->SetTextFont(42);
   PaveInfo->SetFillColor(kBlue-10);
   PaveInfo->SetTextSize(0.03);
   PaveInfo->AddText("The Grover's algorithm is a quantum computing");
   PaveInfo->AddText("algorithm for the search of an element in a");
   PaveInfo->AddText("disordered list, this algorithm have a better");
   PaveInfo->AddText("performance than a classic one, where the");
   PaveInfo->AddText("avarage amount of iterations needed is #alpha N.");
   PaveInfo->AddText("While Grover's algorithm finds the desired");
   PaveInfo->AddText("element in an avarage #alpha#sqrt{N} iterations where N");
   PaveInfo->AddText("is the total amount of elements in the list.");
   PaveInfo->AddText("Grover's algortihm is one of the most mention");
   PaveInfo->AddText("quantum algorithms in the literature.");
   PaveInfo->AddText("In this GUI you will have a deep look into");
   PaveInfo->AddText("Grover's algorithm, getting to know in detail");
   PaveInfo->AddText("the procedure, construction of operators, circuit, and");
   PaveInfo->AddText("geometric visualization, and a visual and interactive");
   PaveInfo->AddText("interface where you can see the step by step of the");
   PaveInfo->AddText("application of the Grover's algorithm");
   PaveInfo->Draw();

   TPaveText *PaveT1 = new TPaveText(0.45,0.35,0.95,0.9);
   PaveT1->SetTextAlign(13);
   PaveT1->SetTextFont(131);
   PaveT1->SetTextSize(0.03);
   PaveT1->SetTextColor(0);
   PaveT1->SetFillColor(kAzure+1);
   PaveT1->AddText("This is a GUI for the Grover's Algorithm learning.");
   PaveT1->AddText("On the left you will find boxes in which you can select");
   PaveT1->AddText("the number of elements to search and the one you want");
   PaveT1->AddText("to search for, after that you can press the Start");
   PaveT1->AddText("button to begin the simulation of the algorithm, the");
   PaveT1->AddText("Restart button if you want to begin a new simulation");
   PaveT1->AddText("and the Close button for exiting the program.");
   PaveT1->Draw();
  

   TPaveText *pavecred = new TPaveText(0.65,0.01,0.99,0.3);
   pavecred->SetTextAlign(10);
   pavecred->SetTextFont(42);
   pavecred->SetTextColor(0);
   pavecred->SetTextSize(0.025);
   pavecred->SetFillColor(kBlue-6);
   
   pavecred->AddText(" ");
   pavecred->AddText(" Authors: ");
   pavecred->AddText(" #font[62]{Daniel M Martin R}");
   pavecred->AddText(" GRUPO DE INVESTIGACION FISINFOR ");
   pavecred->AddText(" #font[62]{Julian A Salamanca B}, PHD ");
   pavecred->AddText(" LIDER GRUPO DE INVESTIGACION FISINFOR ");
   pavecred->AddText(" FACULTAD DE CIENCIAS Y EDUCACION");
   pavecred->AddText(" Profesor #font[72]{UNIVERSIDAD DISTRITAL}");
   pavecred->AddText(" #font[72]{FRANCISCO JOSE DE CALDAS}");

   pavecred->Draw();
      
   MainF->MapSubwindows();
   MainF->Resize();
   MainF->MapWindow();

   
}

void MyMainFrame::DoDraw() {

  next->Disconnect("Clicked()");
  next->Connect("Clicked()","MyMainFrame",this,"Step1()");
  
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  
  //TButton1->SetState(kButtonDisabled, kTRUE);
  NEntry1->SetState(kFALSE);
  NEntry2->SetState(kFALSE);
  
  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.5,0.9, "Geometric Visualization");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  
  N = NEntry1->GetNumber();
  k = NEntry2->GetNumber();
  n = ceil(log2(N));
  
  len = pow(2,n);
  
  double s[N];

  for(int i; i<=N; i++){
    
    s[i]=i+1;
    cout<<s[i]<<endl;
  }
  ms[N];

  for(int i=0; i<=N; i++){

    ms[i]=0;
  }
  
  ms[0]=1;
  
  TMatrixT<double> State(N,1,s);
  //State.Print();
  
  PEstados = new TGraph(N,s,ms);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(22);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  double R = round((sqrt(N)*TMath::Pi()/4.)-0.5);
  RValue->SetNumber(R);
  
  if(k>N){
    cout<<"Number of Elements must be greater than Search Element"<<endl;
  }
  
  switch(n){
  case 2:
    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.7,0.8,0.7);
    Circuit2 = new TLine(0.06,0.3,0.8,0.3);
    
    
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.685,"|0#GT");
    latex.DrawLatex(0.02,0.285,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");
     
    fCanvas->Update();
    
    break;
  case 3:
    
    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.75,0.8,0.75);
    Circuit2 = new TLine(0.06,0.5,0.8,0.5);
    Circuit3 = new TLine(0.06,0.25,0.8,0.25);
    
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.735,"|0#GT");
    latex.DrawLatex(0.02,0.485,"|0#GT");
    latex.DrawLatex(0.02,0.235,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");
     
    fCanvas->Update();
    break;
  case 4:

    fCanvas->cd(1);
    
    Circuit1 = new TLine(0.06,0.75,0.8,0.75);
    Circuit2 = new TLine(0.06,0.55,0.8,0.55);
    Circuit3 = new TLine(0.06,0.35,0.8,0.35);
    Circuit4 = new TLine(0.06,0.15,0.8,0.15);
    
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.735,"|0#GT");
    latex.DrawLatex(0.02,0.535,"|0#GT");
    latex.DrawLatex(0.02,0.335,"|0#GT");
    latex.DrawLatex(0.02,0.135,"|0#GT");
    
    C->Draw();
    
    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();
    Circuit4->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");
     
    fCanvas->Update();
    break;
  case (5):
    cout << ">Four Qubits"<<endl;
    break;
  }
}
//DoDraw End

//.............................................................................

// Step 1 Begin

void MyMainFrame::Step1(){
  
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  GateColor->SetRGB(0.22,0.33,0.61);

  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.5,0.9, "Geometric Visualization");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  for(int i; i<=N; i++){
    
    ms[i]=1/sqrt(N);
    //cout<<ms[i]<<endl;
  }
  
  double s[N];
  
  for(int i; i<=N; i++){
    
    s[i]=i+1;
    cout<<s[i]<<endl;
  }
  
  PEstados = new TGraph(N,s,ms);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(22);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  double xf=sqrt((N-1)*(1./N));
  double yf=sqrt(1./N);
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);

  
  if(k>N){
    cout<<"Number of Elements must be greater than Search Element"<<endl;
  }
  
  switch(n){
  case 2:
     fCanvas->cd(1);
     Circuit1 = new TLine(0.06,0.7,0.8,0.7);
     Circuit2 = new TLine(0.06,0.3,0.8,0.3);
     
     HGate = new TPaveText(0.1,0.624,0.18,0.774);
     HGate->SetTextAlign(22);
     HGate->SetShadowColor(0);
     HGate->SetTextFont(11);
     HGate->SetTextSize(0.07);
     HGate->SetFillColor(20);
     HGate->AddText("H");
     
     HGate2 = new TPaveText(0.1,0.224,0.18,0.374);
     HGate2->SetTextAlign(22);
     HGate2->SetShadowColor(0);
     HGate2->SetTextFont(11);
     HGate2->SetTextSize(0.07);
     HGate2->SetFillColor(20);
     HGate2->AddText("H");
     
     latex.SetTextSize(0.05);
     latex.DrawLatex(0.02,0.685,"|0#GT");
     latex.DrawLatex(0.02,0.285,"|0#GT");
     
     Circuit1->Draw();
     Circuit2->Draw();
     HGate->Draw();
     HGate2->Draw();
     //C->Draw();
     
     fCanvas->cd(2);
     
     Steps->Draw();
     
     fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf,yf,"|#varphi#GT"); 
     
     Axis1->Draw();
     Axis2->Draw();
     SVector->Draw();
     BS->Draw();
     
     fCanvas->cd(4);
     
     PEstados->Draw("AB");
     
     fCanvas->Update();
     
     break;
  case 3:
    
    fCanvas->cd(1);

    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.8,0.75);
    Circuit2 = new TLine(0.06,0.5,0.8,0.5);
    Circuit3 = new TLine(0.06,0.25,0.8,0.25);
    
    HGate = new TPaveText(0.1,0.674,0.18,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.1,0.424,0.18,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.1,0.174,0.18,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.735,"|0#GT");
    latex.DrawLatex(0.02,0.485,"|0#GT");
    latex.DrawLatex(0.02,0.235,"|0#GT");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf,yf,"|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");
     
    fCanvas->Update();
     break;
  case 4:
    cout << "Four Qubits"<<endl;
     break;
  case (5):
    cout << ">Four Qubits"<<endl;
    break;
  }

  next->Disconnect("Clicked()");
  previous->Disconnect("Clicked()");
  next->Connect("Clicked()","MyMainFrame",this,"Oracle()");
  previous->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
}
// End Step 1

//..............................................................................

//Begin Oracle

void MyMainFrame::Oracle(){
  
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  GateColor->SetRGB(0.22,0.33,0.61);
  
  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.5,0.9, "Geometric Visualization");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);

  for(int i; i<=N; i++){
    
    ms[i]=1/sqrt(N);
    //cout<<ms[i]<<endl;
  }
  
  double uf[N*N];
  
  for(int i; i<=(N*N); i++){
    uf[i]=0.0;
    
    cout<<uf[0]<<endl;
  }
  
  for(int i=1; i<=N; i++){
    
    int x = (i-1)+(N*(i-1)); 
    
    uf[x]=1;
    
  }
  
  int s_e = (k-1)+(N*(k-1));
  
  uf[s_e]=-1;
  
  TMatrixT<double> Uf(N,N,uf);
  Uf.Print();

  double s[N];
  
  for(int i; i<=N; i++){
    
    s[i]=i+1;
    cout<<s[i]<<endl;
  }

  TMatrixT<double> State(N,1,ms);

  TMatrixT<double> OResult = Uf*State;

  double *Res = OResult.GetMatrixArray();

   for(int i; i<=N; i++){
    
    ms[i]=Res[i];
    cout<<ms[i]<<endl;
  }

  PEstados = new TGraph(N,s,Res);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(22);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Apply the Diffusion transform D");
  Steps->AddText("Measure the states (qubits)");

  double xf=sqrt((N-1)*(1./N));
  double yf=-sqrt(1./N);
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  
  switch(n){
  case 2:
    fCanvas->cd(1);

    Circuit1 = new TLine(0.06,0.7,0.8,0.7);
    Circuit2 = new TLine(0.06,0.3,0.8,0.3);
    
    HGate = new TPaveText(0.1,0.624,0.18,0.774);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.1,0.224,0.18,0.374);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");
    
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.685,"|0#GT");
    latex.DrawLatex(0.02,0.285,"|0#GT");
    
    UfGate = new TPaveText(0.22,0.624,0.30,0.774);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(20);
    UfGate->AddText("Uf");
    
    UfGate2 = new TPaveText(0.22,0.224,0.30,0.374);
    UfGate2->SetTextAlign(22);
    UfGate2->SetShadowColor(0);
    UfGate2->SetTextFont(11);
    UfGate2->SetTextSize(0.07);
    UfGate2->SetFillColor(20);
    UfGate2->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    UfGate2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf-0.03,yf-0.12,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    
    break;
  case 3:
    
    fCanvas->cd(1);

    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.8,0.75);
    Circuit2 = new TLine(0.06,0.5,0.8,0.5);
    Circuit3 = new TLine(0.06,0.25,0.8,0.25);
    
    HGate = new TPaveText(0.1,0.674,0.18,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.1,0.424,0.18,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.1,0.174,0.18,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.735,"|0#GT");
    latex.DrawLatex(0.02,0.485,"|0#GT");
    latex.DrawLatex(0.02,0.235,"|0#GT");

    UfGate = new TPaveText(0.22,0.674,0.30,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(20);
    UfGate->AddText("Uf");
    
    UfGate2 = new TPaveText(0.22,0.424,0.30,0.574);
    UfGate2->SetTextAlign(22);
    UfGate2->SetShadowColor(0);
    UfGate2->SetTextFont(11);
    UfGate2->SetTextSize(0.07);
    UfGate2->SetFillColor(20);
    UfGate2->AddText("Uf");

    UfGate3 = new TPaveText(0.22,0.174,0.30,0.324);
    UfGate3->SetTextAlign(22);
    UfGate3->SetShadowColor(0);
    UfGate3->SetTextFont(11);
    UfGate3->SetTextSize(0.07);
    UfGate3->SetFillColor(20);
    UfGate3->AddText("Uf");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
    UfGate2->Draw();
    UfGate3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf-0.05,yf-0.05,"U_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    BS->Draw();
    SVector->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    break;
  case 4:
    cout << "Four Qubits"<<endl;
    break;
  case (5):
    cout << ">Four Qubits"<<endl;
    break;
  }

  previous->Disconnect("Clicked()");
  next->Disconnect("Clicked()");
  previous->Connect("Clicked()","MyMainFrame",this,"Step1()");
  next->Connect("Clicked()","MyMainFrame",this,"Diffuser()");
}
// End Orcale

//..............................................................................

//Begin Diffuser

void MyMainFrame::Diffuser(){

  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  GateColor->SetRGB(0.22,0.33,0.61);
  
  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);

  TText *BS = new TText(-0.5,0.9, "Geometric Visualization");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  double dif[N*N];
  
  for(int i; i<=(N*N); i++){
    dif[i]=(2./N);
    //cout<<uf[0]<<endl;
  }
  
  for(int i=1; i<=N; i++){
    
    int x = (i-1)+(N*(i-1)); 
    
    dif[x]=-1+(2./N);
    
  }
  
  TMatrixT<double> Difusser(N,N,dif);
  Difusser.Print();
  
  double s[N];
  
  for(int i; i<=N; i++){
    
    s[i]=i+1;
    cout<<s[i]<<endl;
  }

  TMatrixT<double> State(N,1,ms);

  TMatrixT<double> DResult = Difusser*State;

  double *Res = DResult.GetMatrixArray();

   for(int i; i<=N; i++){
    
    ms[i]=Res[i];
    cout<<ms[i]<<endl;
  }

  PEstados = new TGraph(N,s,Res);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Amplitude");
  
  Steps = new TPaveText(0.2,0.1,0.8,0.9);
  Steps->SetTextAlign(22);
  Steps->SetShadowColor(0);
  Steps->SetFillColor(0);
  Steps->SetLineColor(0);
  Steps->SetTextSize(0.05);
  Steps->AddText("Initialize the n qubits to state |0#GT"); 
  Steps->AddText("Apply Hadamard Gate over the n qubits:");
  Steps->AddText("H^{#otimesn}|0#GT^{#otimesn}");
  Steps->AddText("Apply a sign shift with the oracle U_{f} ");
  Steps->AddText("Apply the Diffusion transform D");((TText*)Steps->GetListOfLines()->Last())->SetTextColor(kOrange+1);
  Steps->AddText("Measure the states (qubits)");

  double xf;
  double yf;
  if(k<N){
    xf=sqrt(ms[k]*ms[k]*(N-1));
    yf=ms[k-1];
  }
  else{
    xf=sqrt(ms[0]*ms[0]*(N-1));
    yf=ms[k-1];
  }
  SVector = new TArrow(0.0,0.0,xf,yf,0.015,"|>");
  SVector->SetFillColor(2);
  SVector->SetLineColor(2);
  
  switch(n){
  case 2:
    fCanvas->cd(1);

    Circuit1 = new TLine(0.06,0.7,0.8,0.7);
    Circuit2 = new TLine(0.06,0.3,0.8,0.3);
    
    HGate = new TPaveText(0.1,0.624,0.18,0.774);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.1,0.224,0.18,0.374);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");
    
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.685,"|0#GT");
    latex.DrawLatex(0.02,0.285,"|0#GT");
    
    UfGate = new TPaveText(0.22,0.624,0.30,0.774);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(20);
    UfGate->AddText("Uf");
    
    UfGate2 = new TPaveText(0.22,0.224,0.30,0.374);
    UfGate2->SetTextAlign(22);
    UfGate2->SetShadowColor(0);
    UfGate2->SetTextFont(11);
    UfGate2->SetTextSize(0.07);
    UfGate2->SetFillColor(20);
    UfGate2->AddText("Uf");

    
    DifGate = new TPaveText(0.34,0.624,0.42,0.774);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(20);
    DifGate->AddText("D");
    
    DifGate2 = new TPaveText(0.34,0.224,0.42,0.374);
    DifGate2->SetTextAlign(22);
    DifGate2->SetShadowColor(0);
    DifGate2->SetTextFont(11);
    DifGate2->SetTextSize(0.07);
    DifGate2->SetFillColor(20);
    DifGate2->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();
    HGate->Draw();
    HGate2->Draw();
    UfGate->Draw();
    UfGate2->Draw();
    
    DifGate->Draw();
    DifGate2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    BS->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    
    break;
  case 3:
    fCanvas->cd(1);

    C->Draw();
    
    Circuit1 = new TLine(0.06,0.75,0.8,0.75);
    Circuit2 = new TLine(0.06,0.5,0.8,0.5);
    Circuit3 = new TLine(0.06,0.25,0.8,0.25);
    
    HGate = new TPaveText(0.1,0.674,0.18,0.824);
    HGate->SetTextAlign(22);
    HGate->SetShadowColor(0);
    HGate->SetTextFont(11);
    HGate->SetTextSize(0.07);
    HGate->SetFillColor(20);
    HGate->AddText("H");
    
    HGate2 = new TPaveText(0.1,0.424,0.18,0.574);
    HGate2->SetTextAlign(22);
    HGate2->SetShadowColor(0);
    HGate2->SetTextFont(11);
    HGate2->SetTextSize(0.07);
    HGate2->SetFillColor(20);
    HGate2->AddText("H");

    HGate3 = new TPaveText(0.1,0.174,0.18,0.324);
    HGate3->SetTextAlign(22);
    HGate3->SetShadowColor(0);
    HGate3->SetTextFont(11);
    HGate3->SetTextSize(0.07);
    HGate3->SetFillColor(20);
    HGate3->AddText("H");

    latex.SetTextSize(0.05);
    latex.DrawLatex(0.02,0.735,"|0#GT");
    latex.DrawLatex(0.02,0.485,"|0#GT");
    latex.DrawLatex(0.02,0.235,"|0#GT");

    UfGate = new TPaveText(0.22,0.674,0.30,0.824);
    UfGate->SetTextAlign(22);
    UfGate->SetShadowColor(0);
    UfGate->SetTextFont(11);
    UfGate->SetTextSize(0.07);
    UfGate->SetFillColor(20);
    UfGate->AddText("Uf");
    
    UfGate2 = new TPaveText(0.22,0.424,0.30,0.574);
    UfGate2->SetTextAlign(22);
    UfGate2->SetShadowColor(0);
    UfGate2->SetTextFont(11);
    UfGate2->SetTextSize(0.07);
    UfGate2->SetFillColor(20);
    UfGate2->AddText("Uf");

    UfGate3 = new TPaveText(0.22,0.174,0.30,0.324);
    UfGate3->SetTextAlign(22);
    UfGate3->SetShadowColor(0);
    UfGate3->SetTextFont(11);
    UfGate3->SetTextSize(0.07);
    UfGate3->SetFillColor(20);
    UfGate3->AddText("Uf");
    
    DifGate = new TPaveText(0.34,0.674,0.42,0.824);
    DifGate->SetTextAlign(22);
    DifGate->SetShadowColor(0);
    DifGate->SetTextFont(11);
    DifGate->SetTextSize(0.07);
    DifGate->SetFillColor(20);
    DifGate->AddText("D");
    
    DifGate2 = new TPaveText(0.34,0.424,0.42,0.574);
    DifGate2->SetTextAlign(22);
    DifGate2->SetShadowColor(0);
    DifGate2->SetTextFont(11);
    DifGate2->SetTextSize(0.07);
    DifGate2->SetFillColor(20);
    DifGate2->AddText("D");

    DifGate3 = new TPaveText(0.34,0.174,0.42,0.324);
    DifGate3->SetTextAlign(22);
    DifGate3->SetShadowColor(0);
    DifGate3->SetTextFont(11);
    DifGate3->SetTextSize(0.07);
    DifGate3->SetFillColor(20);
    DifGate3->AddText("D");

    Circuit1->Draw();
    Circuit2->Draw();
    Circuit3->Draw();

    HGate->Draw();
    HGate2->Draw();
    HGate3->Draw();
    
    UfGate->Draw();
    UfGate2->Draw();
    UfGate3->Draw();
    
    DifGate->Draw();
    DifGate2->Draw();
    DifGate3->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3)->Range(-1,-1,1,1);
    
    Axis1 = new TLine(0.0,0.0,0.9,0.0);
    Axis2 = new TLine(0.0,-0.9,0.0,0.85);
    //Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(-0.07,0.89,"|#beta#GT");
    latex.DrawLatex(0.92,-0.02,"|#alpha#GT");
    latex.DrawLatex(xf+0.02,yf-0.08,"DU_{f}|#varphi#GT");
    
    Axis1->Draw();
    Axis2->Draw();
    SVector->Draw();
    BS->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    break;
  case 4:
    cout << "Four Qubits"<<endl;
    break;
  case (5):
    cout << ">Four Qubits"<<endl;
    break;
  }

  previous->Disconnect("Clicked()");
  next->Disconnect("Clicked()");
  previous->Connect("Clicked()","MyMainFrame",this,"Oracle()");

}

/*-------------------------------------------------------------------
---------------------------------------------------------------------
---------------------BASIC CONCEPTS FUNCTIONS------------------------
---------------------------------------------------------------------
-------------------------------------------------------------------*/

void MyMainFrame::GateConstruct(){
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Divide(3,2);
  fCanvas->Update();

  TText *oracle = new TText(0.5,0.95,"ORACLE Uf");
  oracle->SetTextAlign(22);
  oracle->SetTextColor(kBlue+2);
  oracle->SetTextFont(32);
  oracle->SetTextSize(0.07);

  TText *difusser = new TText(0.5,0.97,"DIFFUSER OPERATOR");
  difusser->SetTextAlign(22);
  difusser->SetTextColor(kBlue+2);
  difusser->SetTextFont(32);
  difusser->SetTextSize(0.07);
  
  fCanvas->cd(1);
  oracle->Draw();
  latex.SetTextSize(0.06);
  latex.DrawLatex(0.02,0.8,"#font[11]{The oracle U_{f} #color[50]{implements a phase shift to the}}");
  latex.DrawLatex(0.02,0.75,"#font[11]{#color[50]{state we are searching for}, lets call this state #color[4]{K}.}");
  latex.DrawLatex(0.02,0.7,"#font[11]{In this context we can define:}");
  latex.DrawLatex(0.02,0.62,"#font[11]{U_{f}=(-1)^{f(x)} where f(x) is given by:}");
  latex.DrawLatex(0.3,0.48,"#font[11]{f(x)=}");
  latex.SetTextSize(0.15);
  latex.DrawLatex(0.4,0.46,"#font[11]{{}");
  latex.SetTextSize(0.06);
  latex.DrawLatex(0.45,0.53,"#font[11]{0 if x #neq #color[4]{K}}");
  latex.DrawLatex(0.43,0.43,"#font[11]{1 if x = #color[4]{K}}");
  latex.DrawLatex(0.02,0.35,"#font[11]{In a way that, when the operator (gate) is applied}");
  latex.DrawLatex(0.02,0.30,"#font[11]{to the given state, all states will remain the same,}");
  latex.DrawLatex(0.02,0.25,"#font[11]{#color[50]{except for the} #color[4]{K} #color[50]{state which will be flipped to a}}");
  latex.DrawLatex(0.02,0.2,"#font[11]{#color[50]{negative phase}, diferenciating it from the rest.}");

  SepLine = new TLine(0,0.01,0.99,0.01);
  SepLine->SetLineColor(2);
  SepLine->Draw();

  fCanvas->cd(2);

  latex.DrawLatex(0.02,0.85,"#font[10]{Lets suppose the next superposition state:}");
  latex.DrawLatex(0.02,0.75,"#font[11]{|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT + #upoint#upoint#upoint}");
  latex.DrawLatex(0.02,0.66,"#font[11]{+i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex.DrawLatex(0.02,0.56,"#font[11]{Applying the U_{f} gate, to the state, gives as result:}");
  latex.DrawLatex(0.02,0.46,"#font[11]{(-1)^{f(x)}|#varphi#GT=#frac{1}{#sqrt{N}}(a|00#upoint#upoint#upoint 0#GT + b|00#upoint#upoint#upoint 1#GT +}");
  latex.DrawLatex(0.02,0.37,"#font[11]{#upoint#upoint#upoint #color[2]{-}i|#color[4]{K}#GT + #upoint#upoint#upoint + v|11#upoint#upoint#upoint 1#GT)}");
  latex.DrawLatex(0.02,0.30, "#font[11]{The U_{f} gate can be written in the matrix form as:}");
  latex.DrawLatex(0.02,0.13,"#font[11]{U_{f}=}");
  latex.SetTextSize(0.27);
  latex.DrawLatex(0.12,0.1,"#font[11]{(}");
  latex.DrawLatex(0.89,0.1,"#font[11]{)}");
  latex.SetTextSize(0.06);
  latex.DrawLatex(0.2,0.22,"#font[11]{1-2f(0)          0         #upoint#upoint#upoint         0}");
  latex.DrawLatex(0.215,0.16,"#font[11]{  0            1-2f(1)      #upoint#upoint#upoint         0}");
  latex.SetTextAngle(90);
  latex.DrawLatex(0.26,0.11,"#font[11]{#upoint#upoint#upoint}");
  latex.DrawLatex(0.48,0.11,"#font[11]{#upoint#upoint#upoint}");
  latex.DrawLatex(0.815,0.11,"#font[11]{#upoint#upoint#upoint}");
  latex.SetTextAngle(158);
  latex.DrawLatex(0.71,0.12,"#font[11]{#upoint #upoint #upoint}");
  latex.SetTextAngle(0);
  latex.DrawLatex(0.19,0.055,"#font[11]{    0                0         #upoint#upoint#upoint    1-2f(N)}");

  SepLine->Draw();


  fCanvas->cd(3);

  latex.DrawLatex(0.02,0.8,"#font[10]{We can, aswell, see that #color[50]{the U_{f} operator is}}");
  latex.DrawLatex(0.02,0.73,"#font[10]{#color[50]{equivalent} to the operation #color[51]{I-2|#color[4]{K}#GT#LT#color[4]{K}|}}");
  latex.DrawLatex(0.20,0.64,"#font[10]{#color[51]{(I-2|#color[4]{K}#GT#LT#color[4]{K}|)} |#varphi#GT=I|#varphi#GT-2|#color[4]{K}#GT#LT#color[4]{K}|#varphi#GT}");
  latex.DrawLatex(0.02,0.56,"#font[10]{where #LTk|#varphi#GT is #color[50]{the inside product}, which is equal}");
 latex.DrawLatex(0.02,0.51,"#font[10]{to 0 if #varphi#neq#color[4]{K} an 1 if #varphi=#color[4]{K}}");
 latex.DrawLatex(0.02,0.46,"#font[10]{Lets rewrite the |#varphi#GT state as |#phi#GT+|#color[4]{K}#GT}");
  latex.DrawLatex(0.20,0.37,"#font[10]{=I(|#phi#GT+|#color[4]{K}#GT)-2|k#GT#LT#color[4]{K}|(|#phi#GT+|#color[4]{K}#GT)}");
  latex.DrawLatex(0.20,0.29,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT(#LT#color[4]{K}|#phi#GT+#LT#color[4]{K}|#color[4]{K}#GT)}");
  latex.DrawLatex(0.20,0.21,"#font[10]{=(|#phi#GT+|#color[4]{K}#GT)-2|#color[4]{K}#GT}");
  latex.DrawLatex(0.20,0.13,"#font[10]{=|#phi#GT#color[2]{-}|#color[4]{K}#GT}");
  latex.DrawLatex(0.02,0.06,"#font[10]{We can see that the |#color[4]{K}#GT state sign was flipped.}");

  SepLine->Draw();
  
  fCanvas->cd(4);
  
  difusser->Draw();
  latex.DrawLatex(0.02,0.88,"#font[11]{The diffuser operator is defined, by Grover, in its}");
  latex.DrawLatex(0.02,0.83,"#font[11]{matrix form, as follow:}");
  latex.DrawLatex(0.2,0.75,"#font[11]{#color[2]{D_{ij}=#frac{2}{N}  with i#neqj} and #color[4]{D_{ii}=-1+#frac{2}{N}}}");
  latex.DrawLatex(0.02,0.68,"#font[11]{Giving as result the matrix:}");
  latex.DrawLatex(0.04,0.47,"#font[11]{D=}");
  latex.SetTextSize(0.43);
  latex.DrawLatex(0.11,0.38,"#font[11]{(}");
  latex.DrawLatex(0.9,0.38,"#font[11]{)}");
  latex.SetTextSize(0.06);
  latex.DrawLatex(0.2,0.58,"#font[11]{-1+#frac{2}{N}         #frac{2}{N}       #upoint#upoint#upoint      #frac{2}{N}}");
  latex.DrawLatex(0.215,0.48,"#font[11]{ #frac{2}{N}             -1+#frac{2}{N}      #upoint#upoint#upoint      #frac{2}{N}}");
  latex.SetTextAngle(90);
  latex.DrawLatex(0.26,0.39,"#font[11]{#upoint#upoint#upoint}");
  latex.DrawLatex(0.515,0.39,"#font[11]{#upoint#upoint#upoint}");
  latex.DrawLatex(0.84,0.39,"#font[11]{#upoint#upoint#upoint}");
  latex.SetTextAngle(158);
  latex.DrawLatex(0.71,0.39,"#font[11]{#upoint #upoint #upoint}");
  latex.SetTextAngle(0);
  latex.DrawLatex(0.19,0.31,"#font[11]{  #frac{2}{N}          #frac{2}{N}       #upoint#upoint#upoint       -1+#frac{2}{N}}");
  latex.DrawLatex(0.02,0.13,"#font[11]{Where we can see #color[50]{only the diagonal elements are}}");
  latex.DrawLatex(0.02,0.08,"#font[11]{#color[50]{different from the others}. Watching the matrix}");
  latex.DrawLatex(0.02,0.03,"#font[11]{obtained, we can notice this could be rewritten}");

  fCanvas->cd(5);
  latex.DrawLatex(0.02,0.9,"#font[11]{as the sum of two matrices: #color[9]{-I+#frac{2}{N}J} where #color[50]{I is}}");
  latex.DrawLatex(0.02,0.83,"#font[11]{#color[50]{the identity matrix and J is an all ones matrix.}}");
  latex.DrawLatex(0.02,0.77,"#font[11]{The diffuser operator can also be rewritten as the}");
  latex.DrawLatex(0.02,0.71,"#font[11]{operation #color[51]{H^{#otimesn}RH^{#otimesn}} or #color[51]{H^{#otimesn}(2|0#GT^{#otimesn}#LT0|^{#otimesn}-I_{n})H^{#otimesn}}}");
  latex.DrawLatex(0.02,0.65,"#font[11]{Where #color[50]{H is the Hadamard gate} and #color[50]{R is -I except}}");
  latex.DrawLatex(0.02,0.59,"#font[11]{#color[50]{in R_{00}=1}. We can express the matrix R as two}");
  latex.DrawLatex(0.02,0.53,"#font[11]{different matrices #color[833]{R_{1}=-I} and #color[833]{R_{2,00}=2 and R_{2,ij}=0}}");
  latex.DrawLatex(0.02,0.47,"#font[11]{having as a result the operation #color[51]{H^{#otimesn}R_{1}H^{#otimesn}+}}");
  latex.DrawLatex(0.02,0.41,"#font[11]{#color[51]{H^{#otimesn}R_{2}H^{#otimesn}}. The first operation is easy, being:}");
  latex.DrawLatex(0.02,0.35,"#font[11]{#color[51]{H^{#otimesn}}#color[833]{(-I)}#color[51]{H^{#otimesn}}=#color[51]{H^{#otimesn}(#color[2]{-}H^{#otimesn})}=#color[808]{-I} #color[50]{Since H is unitary}}");
  latex.DrawLatex(0.02,0.29,"#font[11]{#color[50]{HH=I}. On the other hand, the second operation is}");
  latex.DrawLatex(0.02,0.23,"#font[11]{a little bit harder, for this one we will rewrite}");
  latex.DrawLatex(0.02,0.16,"#font[11]{the Hadamard gate as follow: #color[4]{H_{ij}=2^{-n/ 2}(-1)^{i #upoint j}}}");
  latex.DrawLatex(0.02,0.09,"#font[11]{#color[50]{Where i #upoint j is the bitwise dot product}, which}");
  latex.DrawLatex(0.02,0.03,"#font[11]{compares the amount of 1's the bit i shares with}");

  fCanvas->cd(6);
  
  latex.DrawLatex(0.02,0.9,"#font[11]{the bit j, #color[50]{if this amount is odd i #upoint j=1, else it's 0.}}");
  latex.DrawLatex(0.02,0.85,"#font[11]{In order to find this second operation we make an}");
  latex.DrawLatex(0.02,0.8,"#font[11]{standard matrix multiplication:}");
  latex.DrawLatex(0.02,0.71,"#font[11]{#color[51]{H^{#otimesn}}#color[833]{R_{2}}#color[51]{H^{#otimesn}}=#sum}#font[11]{#color[51]{H_{ab}}#color[833]{R_{2, bc}}#color[51]{H_{cd}}}");
  latex.DrawLatex(0.02,0.63,"#font[11]{given the definition of #color[833]{R_{2}} we obtain:}");
  latex.DrawLatex(0.02,0.54,"#font[11]{=#color[51]{H_{a0}}#color[833]{R_{2, 00}}#color[51]{H_{0d}}=2#frac{1}{2^{n/ 2}}(-1)^{a#upoint 0}#frac{1}{2^{n/ 2}}(-1)^{0#upoint d}}");
  latex.DrawLatex(0.02,0.46,"#font[11]{=#frac{2}{2^{n}}}");
  latex.DrawLatex(0.02,0.38,"#font[11]{But 2^{n}=N, so that #color[51]{H^{#otimesn}}#color[833]{R_{2}}#color[51]{H^{#otimesn}}=#color[808]{#frac{2}{N}}}");
  latex.DrawLatex(0.02,0.31,"#font[11]{Adding up the two obtained terms we get: #color[9]{-I+#frac{2}{N}}}");
  latex.DrawLatex(0.02,0.24,"#font[11]{which is the same operation as the diffuser gate}");
  latex.DrawLatex(0.02,0.18,"#font[11]{#color[50]{The diffuser operator is known as an invertion}}");
  latex.DrawLatex(0.02,0.12,"#font[11]{#color[50]{over the avarage}, this because it takes the state}");
  latex.DrawLatex(0.02,0.06,"#font[11]{previously shift and shifts it again with a #color[50]{bigger}}");
  latex.DrawLatex(0.02,0.01,"#font[11]{#color[50]{amplitude. Leaving the avarage unchanged.}}");
  
  fCanvas->Update();
  
}

void MyMainFrame::Algorithm(){
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Divide(2,1);

  fCanvas->cd(1);

  
  
  fCanvas->Update();
}


void MyMainFrame::RDefinition(){
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Update();
}



void InterfaceCopy() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),800,450);
}

