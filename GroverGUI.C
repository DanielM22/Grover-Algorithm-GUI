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
  TGTextButton        *TButton1, *TButton2, *TButton3, *next, *previous;
  TGNumberEntry       *NEntry1, *NEntry2, *RValue;;
  TGLabel             *Label1, *Label2, *Label3;
  TGGroupFrame        *GFrame1, *GFrame2, *GFrame3;
  TGraph              *PEstados;

  TLine               *Circuit1, *Circuit2, *Axis1, *Axis2, *Axis3;
  TLatex               latex;
  TArrow              *ar2;
  TPaveText           *HGate, *HGate2, *Steps, *UfGate, *UfGate2, *DifGate, *DifGate2;
  TColor              *GateColor = gROOT->GetColor(20);
  TCanvas             *fCanvas;

  double N, k, *M_state;
  int n, len;
  double s[];

  
public:
  MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
  //virtual ~MyMainFrame();
  void DoDraw();
  void Step1();
  void Oracle();
};

MyMainFrame::MyMainFrame(const TGWindow *p,UInt_t w,UInt_t h) {
   // Create a main frame
   MainF = new TGMainFrame(p,w,h);
   MainF->SetWMSizeHints(w,h,1600,900,10,10);
   MainF->SetWindowName("Algoritmo Grover");


   HFrame1 = new TGHorizontalFrame(MainF);
   VFrame1 = new TGHorizontalFrame(MainF);
   HFrame2 = new TGHorizontalFrame(VFrame1);
   VFrame2 = new TGVerticalFrame(HFrame1);
   HFrame3 = new TGHorizontalFrame(VFrame1);

   MCanvas = new TRootEmbeddedCanvas("Lienzo",HFrame1,w,h);

   GFrame1 = new TGGroupFrame(VFrame2,"Number of Elements");
   GFrame2 = new TGGroupFrame(VFrame2, "Search Element");

   TButton1 = new TGTextButton (HFrame2, "Start");
   TButton1->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
   TButton2 = new TGTextButton (HFrame2, "Restart");
   TButton3 = new TGTextButton (HFrame2, "Close");
   next = new TGTextButton (HFrame2, "Next");
   previous = new TGTextButton (HFrame2, "Previous");
  
   NEntry1 = new TGNumberEntry(GFrame1);
   NEntry1->SetLimits(TGNumberFormat::kNELLimitMinMax,3,16);
   
   NEntry2 = new TGNumberEntry(GFrame2);
   NEntry2->SetLimits(TGNumberFormat::kNELLimitMinMax,1,16);
   
   Label3 = new TGLabel(VFrame2, "R Value:");

   RValue = new TGNumberEntry(VFrame2);
   RValue->SetState(kFALSE);

   
   TGLayoutHints *Centrar = new TGLayoutHints (kLHintsCenterX | kLHintsCenterY ,5 ,5 ,5 ,5);
   TGLayoutHints *Expandir = new TGLayoutHints (kLHintsExpandX | kLHintsExpandY ,10 ,10 ,10 ,10);
   TGLayoutHints *CentrarX = new TGLayoutHints (kLHintsCenterX ,5 ,5);
   TGLayoutHints *CentrarY = new TGLayoutHints (kLHintsCenterY ,5 ,5);
   TGLayoutHints *ExpandirX = new TGLayoutHints (kLHintsExpandX ,0 ,10);
   TGLayoutHints *ExpandirY = new TGLayoutHints (kLHintsExpandY ,10 ,10);

   TGIcon *IconUD= new TGIcon(HFrame3, "pictures/LogoUD1.png");
   TGIcon *IFisinfor = new TGIcon(HFrame3, "pictures/logo_fisinfor.png");


   GFrame1->AddFrame(NEntry1,Centrar);
   GFrame2->AddFrame(NEntry2,Centrar);

   VFrame2->AddFrame(GFrame1,CentrarX);
   VFrame2->AddFrame(GFrame2,CentrarX);
   VFrame2->AddFrame(Label3,CentrarX);
   VFrame2->AddFrame(RValue,CentrarX);

   HFrame2->AddFrame(TButton1,new TGLayoutHints(kLHintsCenterY | kLHintsCenterX, 5,5,5,5));
   HFrame2->AddFrame(TButton2,Centrar);
   HFrame2->AddFrame(TButton3,Centrar);
   HFrame2->AddFrame(previous, Centrar);
   HFrame2->AddFrame(next, Centrar);

   
   HFrame3->AddFrame(IFisinfor, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IFisinfor->Resize(70,85);
   HFrame3->AddFrame(IconUD, new TGLayoutHints(kLHintsCenterY, 2,2,2,2));
   IconUD->Resize(70,80);
   

   HFrame1->AddFrame(VFrame2,ExpandirY);
   HFrame1->AddFrame(MCanvas, Expandir);
   VFrame1->AddFrame(HFrame2,new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 2,2,2,2));
   VFrame1->AddFrame(HFrame3,new TGLayoutHints(kLHintsRight, 2,2,0,0));

   MainF->AddFrame(HFrame1,Expandir);
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
  
  next->Connect("Clicked()","MyMainFrame",this,"Step1()");
  
  fCanvas = MCanvas->GetCanvas();
  fCanvas->Clear();
  fCanvas->Update();
  fCanvas->Divide(2,2);
  
  gSystem->Load("LibGeom");
  TSPHE *Bsphe1 = new TSPHE("BSPHE1","BSPHE1","Iron",0,100, 0,180, 0,360);
  Bsphe1->SetLineColor(kGray);
  //TSPHE *Bsphe2 = new TSPHE("BSPHE2","BSPHE2","Iron",0,100, 0,180, 0,360);
  //Bsphe2->SetLineColor(kGray);
  TNode *node1 = new TNode("NODE1","NODE1","BSPHE1",10, 250,300);
  //TNode *node2 = new TNode("NODE2","NODE2","BSPHE2",250, 0,0);
  
  //TButton1->SetState(kButtonDisabled, kTRUE);
  NEntry1->SetState(kFALSE);
  NEntry2->SetState(kFALSE);
  
  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.7,0.9, "Bloch Sphere");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  
  N = NEntry1->GetNumber();
  k = NEntry2->GetNumber();
  n = ceil(log2(N));
  
  len = pow(2,n);
  
  s[len];

  for(int i; i<=len; i++){
    
    s[i]=i+1;
    cout<<s[i]<<endl;
  }
  double ms[len];
  ms[0]=1;
  
  TMatrixT<double> State(len,1,s);
  //State.Print();
  PEstados = new TGraph(N,s,ms);
  PEstados->SetFillColor(40);
  PEstados->SetTitle("States Probability");
  
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

  double R = round(sqrt(N)*TMath::Pi()/4.);
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
    
    fCanvas->cd(3);
    node1->Draw();
    
    BS->Draw();
    
    Axis1 = new TLine(0.0,0.0,0.0,0.9);
    Axis2 = new TLine(0.0,0.0,0.5,0);
    Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(0.01,0.9,"|0#GT");
    latex.DrawLatex(0.01,-0.9,"|1#GT");
    latex.DrawLatex(-0.40,-0.56,"|+#GT");
    latex.DrawLatex(0.32,0.48,"|-#GT");
    
    ar2 = new TArrow(0.0,0.0,0.0,0.75,0.015,"|>");
    ar2->SetFillColor(2);
    ar2->SetLineColor(2);
    ar2->SetAngle(45);
    ar2->Draw();
    
    Axis1->Draw();
    Axis2->Draw();
    Axis3->Draw();
    
    fCanvas->cd(4);
    
    fCanvas->SetTitle("Circuit");
    PEstados->Draw("AB");
     
    fCanvas->Update();
    
    break;
  case 3:
    cout << "Three Qubits"<<endl;
    break;
  case 4:
    cout << "Four Qubits"<<endl;
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
  
  gSystem->Load("LibGeom");
  TSPHE *Bsphe1 = new TSPHE("BSPHE1","BSPHE1","Iron",0,100, 0,180, 0,360);
  Bsphe1->SetLineColor(kGray);
  TNode *node1 = new TNode("NODE1","NODE1","BSPHE1",10, 250,300);

  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.7,0.9, "Bloch Sphere");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  //double N = NEntry1->GetNumber();
  //double k = NEntry2->GetNumber();
  //int n = ceil(log2(N));
  
  //int len = pow(2,n);
  
  double dif[len*len];
  
  for(int i; i<=(len*len); i++){
     dif[i]=(2/N);
     //cout<<uf[0]<<endl;
  }
  
  for(int i=1; i<=len; i++){
     
    int x = (i-1)+(len*(i-1)); 
    
    dif[x]=-1+(2/N);
    
  }
  
   TMatrixT<double> Difusser(len,len,dif);
   //Difusser.Print();
   
   double ms[len];
   
   for(int i; i<=len; i++){
     
     ms[i]=1/sqrt(2);
     //cout<<ms[i]<<endl;
  }
   
   PEstados = new TGraph(N,s,ms);
   PEstados->SetFillColor(40);
   PEstados->SetTitle("States Probability");
   
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
     C->Draw();

     fCanvas->cd(2);
     
     Steps->Draw();
     
     fCanvas->cd(3);
     node1->Draw();
     BS->Draw();
     
     Axis1 = new TLine(0.0,0.0,0.0,0.9);
     Axis2 = new TLine(0.0,0.0,0.5,0);
     Axis3 = new TLine(0,0,-0.316,-0.589);

     latex.SetTextSize(0.06);
     latex.DrawLatex(0.01,0.9,"|0#GT");
     latex.DrawLatex(0.01,-0.9,"|1#GT");
     latex.DrawLatex(-0.40,-0.56,"|+#GT");
     latex.DrawLatex(0.32,0.48,"|-#GT");

     ar2 = new TArrow(0.0,0.0,-0.271,-0.505,0.015,"|>");
     ar2->SetFillColor(2);
     ar2->SetLineColor(2);
     ar2->SetAngle(45);
     ar2->Draw();
     
     Axis1->Draw();
     Axis2->Draw();
     Axis3->Draw();

     fCanvas->cd(4);

     PEstados->Draw("AB");
     
     fCanvas->Update();
     
     break;
   case 3:
     cout << "Three Qubits"<<endl;
     break;
   case 4:
     cout << "Four Qubits"<<endl;
     break;
   case (5):
     cout << ">Four Qubits"<<endl;
     break;
   }

   next->Connect("Clicked()","MyMainFrame",this,"Oracle()");
   previous->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
}
// End Step 1

//..............................................................................

//Begin Oracle

void MyMainFrame::Oracle(){
  
  //fCanvas->Clear();
  fCanvas->Update();
  //fCanvas->Divide(2,2);
  GateColor->SetRGB(0.22,0.33,0.61);
  
  TText *C = new TText(0.5,0.9,"Circuit");
  C->SetTextAlign(22);
  C->SetTextColor(kBlue+2);
  C->SetTextFont(22);
  C->SetTextSize(0.07);
  
  TText *BS = new TText(-0.7,0.9, "Bloch Sphere");
  BS->SetTextAlign(22);
  BS->SetTextColor(kBlue+2);
  BS->SetTextFont(22);
  BS->SetTextSize(0.07);
  
  double uf[len*len];
  
  for(int i; i<=(len*len); i++){
    uf[i]=0.0;
    
    cout<<uf[0]<<endl;
  }
  
  for(int i=1; i<=len; i++){
    
    int x = (i-1)+(len*(i-1)); 
    
    uf[x]=1;
    
  }
  
  int s_e = (k-1)+(len*(k-1));
  
  uf[s_e]=-1;
  
  TMatrixT<double> Uf(len,len,uf);
  Uf.Print();

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
  
  switch(n){
  case 2:
    fCanvas->cd(1);
    
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
    
    UfGate->Draw();
    UfGate2->Draw();
    
    fCanvas->cd(2);
    
    Steps->Draw();
    
    fCanvas->cd(3);
    //node1->Draw();
    BS->Draw();
    
    Axis1 = new TLine(0.0,0.0,0.0,0.9);
    Axis2 = new TLine(0.0,0.0,0.5,0);
    Axis3 = new TLine(0,0,-0.316,-0.589);
    
    latex.SetTextSize(0.06);
    latex.DrawLatex(0.01,0.9,"|0#GT");
    latex.DrawLatex(0.01,-0.9,"|1#GT");
    latex.DrawLatex(-0.40,-0.56,"|+#GT");
    latex.DrawLatex(0.32,0.48,"|-#GT");
    
    ar2 = new TArrow(0.0,0.0,-0.271,-0.505,0.015,"|>");
    ar2->SetFillColor(2);
    ar2->SetLineColor(2);
    ar2->SetAngle(45);
    ar2->Draw();
    
    Axis1->Draw();
    Axis2->Draw();
    Axis3->Draw();
    
    fCanvas->cd(4);
    
    PEstados->Draw("AB");
    
    fCanvas->Update();
    
    break;
  case 3:
    cout << "Three Qubits"<<endl;
    break;
  case 4:
    cout << "Four Qubits"<<endl;
    break;
  case (5):
    cout << ">Four Qubits"<<endl;
    break;
  }

  previous->Connect("Clicked()","MyMainFrame",this,"Step1()");
}

void interface_test() {
  // Popup the GUI...
  new MyMainFrame(gClient->GetRoot(),800,450);
}

