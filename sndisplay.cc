#include "TBox.h"
#include "TText.h"
#include "TLine.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TSystem.h"
#include "TString.h"

#include<vector>

namespace sndisplay
{
  class calorimeter
  {
  public:
    calorimeter (const char *n = "") : name (n)
    {
      canvas = NULL;

      for (unsigned int calo=0; calo<ncalo; ++calo)
	content.push_back(0);

      range_min = range_max = -1;


      const double spacerx = 0.0125;
      const double spacery = 0.0250;

      const double mw_sizey = (1-4*spacery)/(13+2); // (1-2*spacery)/13;
      const double gv_sizey = mw_sizey;
      const double xw_sizey = mw_sizey*13./16.; // (1-4*spacery-2*gv_sizey)/16;

      const double mw_sizex = (0.5-4*spacerx)/(20+4);
      const double gv_sizex = mw_sizex*20./16.;
      const double xw_sizex = mw_sizex;
      
      for (unsigned int mw0=0; mw0<2; ++mw0) {
	for (unsigned int mw1=0; mw1<20; ++mw1) {
	  for (unsigned int mw2=0; mw2<13; ++mw2) {
	    unsigned int id = mw0*20*13 + mw1*13 + mw2;
	    double x1 = spacerx + 2*xw_sizex + spacerx + 0.5*mw0 + mw_sizex*(mw1);
	    if (mw0 == 1) // swap french in case of internal view
	      x1 = spacerx + 2*xw_sizex + spacerx + 0.5*mw0 + mw_sizex*(19-mw1);

	    double y1 = spacery + gv_sizey + spacery + mw_sizey*(mw2);
	    double x2 = x1 + mw_sizex; double y2 = y1 + mw_sizey;
	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);

	    TText *text = new TText(x1+0.33*mw_sizex, y1+0.33*mw_sizey, Form("%03d",id));
	    text->SetTextSize(0.02);
	    omtext.push_back(text);

	    // printf("%d:%02d:%02d has id = %03d (box = %Xd)\n", mw0, mw1, mw2, id, box);
	  }
	}
      }


      for (unsigned int xw0=0; xw0<2; ++xw0) {
	for (unsigned int xw1=0; xw1<2; ++xw1) {
	  for (unsigned int xw2=0; xw2<2; ++xw2) {
	    for (unsigned int xw3=0; xw3<16; ++xw3) {

	    unsigned int id = 520 + xw0*2*2*16 + xw1*2*16 + xw2*16 + xw3;

	    double x1;
	    
	    switch (xw0) {
	    case 0:
	      switch (xw1) {
	      case 0: x1 = spacerx + xw_sizex*xw2; break;
	      case 1: x1 = spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw2)*xw_sizex; break;}
	      break;
	      
	    case 1: // wall ID 0
	      switch (xw1) { // side ID
	      case 1: x1 = 0.5 + spacerx + xw_sizex*xw2; break;
	      case 0: x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + 20*mw_sizex + spacerx + (1-xw2)*xw_sizex; break;}
	      break;}

	    double x2 = x1 + xw_sizex;

	    double y1 = spacery + gv_sizey + spacery + xw_sizey*(xw3);
	    double y2 = spacery + gv_sizey + spacery + xw_sizey*(xw3+1);

	    // double x1 = spacerx + 0.5*xw0 + xw_sizex*(xw1);   double y1 = spacery + xw_sizey*(xw2);
	    // double x2 = spacerx + 0.5*xw0 + xw_sizex*(xw1+1); double y2 = spacery + xw_sizey*(xw2+1);
	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);


	    TText *text = new TText(x1+0.33*mw_sizex, y1+0.33*mw_sizey, Form("%03d",id));
	    text->SetTextSize(0.02);
	    omtext.push_back(text);

	    // printf("%d:%02d:%02d:%02d has id = %03d (box = %Xd) @ (%.5f,%.5f)\n", xw0, xw1, xw2, xw3, id, box, x1, y1);

	    }
	  }
	}
      }

      
      
      for (unsigned int gv0=0; gv0<2; ++gv0) {
	for (unsigned int gv1=0; gv1<2; ++gv1) {
	  for (unsigned int gv2=0; gv2<16; ++gv2) {
	    unsigned int id = 520 + 128 + gv0*2*16 + gv1*16 + gv2;

	    double x1;

	    switch (gv0) {
	    case 0: // side ID 1
	      x1 = spacerx + 2*xw_sizex + spacerx + gv_sizex*gv2;
	      break;

	    case 1: // wall ID 0
	      x1 = 0.5 + spacerx + 2*xw_sizex + spacerx + gv_sizex*(16-1-gv2);
	      break;
	    }

	    double x2 = x1 + gv_sizex;

	    double y1 = spacery + gv1*(gv_sizey + spacery + 13*mw_sizey + spacery);
	    double y2 = y1 + gv_sizey;
	    
	    TBox *box = new TBox(x1, y1, x2, y2);
	    box->SetFillColor(0);
	    box->SetLineWidth(1);
	    ombox.push_back(box);

	    TText *text = new TText(x1+0.33*gv_sizex, y1+0.33*gv_sizey, Form("%03d",id));
	    text->SetTextSize(0.02);
	    omtext.push_back(text);

	    // printf("%d:%02d:%02d has id = %03d (box = %Xd)\n", mw0, mw1, mw2, id, box);
	  }
	}
      }

      source_foil = new TLine (0.5, spacery, 0.5, 1-spacery);
      source_foil->SetLineWidth(2);

      it_label = new TText (spacerx,     spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "  ITALY");
      fr_label = new TText (0.5+spacerx, spacery+gv_sizey+spacery+13*mw_sizey+spacery+0.25*gv_sizey, "FRANCE");

      it_label->SetTextSize(0.036);
      fr_label->SetTextSize(0.036);

      const Int_t nRGBs = 6;
      Double_t stops[nRGBs] = { 0.00, 0.20, 0.40, 0.60, 0.80, 1.00 };
      Double_t red[nRGBs]   = { 0.25, 0.00, 0.20, 1.00, 1.00, 0.90 };
      Double_t green[nRGBs] = { 0.25, 0.80, 1.00, 1.00, 0.80, 0.00 };
      Double_t blue[nRGBs]  = { 1.00, 1.00, 0.20, 0.00, 0.00, 0.00 };
      
      palette_index = TColor::CreateGradientColorTable(nRGBs, stops, red, green, blue, 100);

    };

    ~calorimeter() {};
    
    static const unsigned int nmwall = 520;
    static const unsigned int nxwall = 128;
    static const unsigned int ngveto =  64;

    static const unsigned int ncalo  = 712;

    void setrange(float xmin, float xmax) 
    {
      range_min = xmin; range_max = xmax;
    }

    void draw()
    {
      if (canvas == NULL)
	canvas = new TCanvas (Form("C%s",name.Data()), "SuperNEMO calorimeter", 1750, 500);

      canvas->SetEditable(true);

      canvas->cd();

      for (unsigned int mw0=0; mw0<2; ++mw0) {
	for (unsigned int mw1=0; mw1<20; ++mw1) {
	  for (unsigned int mw2=0; mw2<13; ++mw2) {
	    unsigned int id = mw0*20*13 + mw1*13 + mw2;
	    // printf("omid[%03d] = 0x%Xd\n", id, ombox[id]);
	    ombox[id]->Draw("l");
	    omtext[id]->Draw();
	  }
	}
      }
      
      
      for (unsigned int xw0=0; xw0<2; ++xw0) {
	for (unsigned int xw1=0; xw1<2; ++xw1) {
	  for (unsigned int xw2=0; xw2<2; ++xw2) {
	    for (unsigned int xw3=0; xw3<16; ++xw3) {
	      unsigned int id = 520 + xw0*2*2*16 + xw1*2*16 + xw2*16 + xw3;
	      ombox[id]->Draw("l");
	      omtext[id]->Draw();
	    }
	  }
	}
      }


      for (unsigned int gv0=0; gv0<2; ++gv0) {
	for (unsigned int gv1=0; gv1<2; ++gv1) {
	  for (unsigned int gv2=0; gv2<16; ++gv2) {
	    unsigned int id = 520 + 128 + gv0*2*16 + gv1*16 + gv2;
	    ombox[id]->Draw("l");
	    omtext[id]->Draw();
	  }
	}
      }

      source_foil->Draw();
      it_label->Draw();
      fr_label->Draw();

      canvas->SetEditable(false);
      gSystem->ProcessEvents();
    }
    
    void reset() {
      for (unsigned int calo=0; calo<ncalo; ++calo)
	content[calo] = 0;

      // float content_min = 0; float content_max = 1;

      for (unsigned int calo=0; calo<ncalo; ++calo)
	ombox[calo]->SetFillColor(0);
      
      canvas->Modified();
      canvas->Update();
      gSystem->ProcessEvents();
      
    }


    void setcontent (int pmt, float value) {
      content[pmt] = value;}


    void update() {

      float content_min = content[0];
      float content_max = content[0];

      if (range_min == -1) {
	for (unsigned int calo=1; calo<ncalo; ++calo) {
	  if (content[calo] < content_min) content_min = content[calo];
	  if (content[calo] > content_max) content_max = content[calo];}
      } else {
	range_min = 0; content_max = range_max;}

      for (unsigned int calo=0; calo<ncalo; ++calo)
	if (content[calo] != 0)
	  // ombox[calo]->SetFillColor(TColor::GetColorPalette((int)(99*(content[calo]-content_min)/(content_max-content_min))));
	  ombox[calo]->SetFillColor(palette_index + (int)(99*(content[calo]-content_min)/(content_max-content_min)));
	else
	  ombox[calo]->SetFillColor(0);

      canvas->Modified();
      canvas->Update();
      gSystem->ProcessEvents();

    }
    

    // void setcontent(geomid) 

    // void setcontent (uint32_t geomid0, uint32_t geomid1, uint32_t geomid2, uint32_t geomid3, uint32_t geomid4, float content)
    // {
    //   switch (geomid0)
    // 	{
    // 	  case 
    // 	}
    // }

    TString name;

    float range_min, range_max;

    std::vector<float> content;
    std::vector<TBox*>   ombox;
    std::vector<TText*>  omtext;

    TLine *source_foil;
    TText *it_label;
    TText *fr_label;

    TCanvas *canvas;

    int palette_index;
  };
}

void sndisplay_test ()
{
  sndisplay::calorimeter *sncalo = new sndisplay::calorimeter;

  sncalo->draw();

  sncalo->setcontent(0, 0.001);

  for (int pmt=1; pmt<712; ++pmt)
    sncalo->setcontent(pmt, pmt/7.11);
    // sncalo->ombox[pmt]->SetFillColor(TColor::GetColorPalette((int)(pmt/7.13)));

  sncalo->update();
}

