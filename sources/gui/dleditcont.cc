//
// $Source$
// $Date$
// $Revision$
//
// DESCRIPTION:
// Dialog for viewing and editing properties of a contingency
//

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif  // WX_PRECOMP
#include "game/nfg.h"
#include "dleditcont.h"

//======================================================================
//                   class dialogEditContingency
//======================================================================

dialogEditContingency::dialogEditContingency(wxWindow *p_parent,
					     const Nfg &p_nfg,
					     const gArray<int> &p_cont)
  : wxDialog(p_parent, -1, "Contingency properties")
{
  SetAutoLayout(true);

  wxBoxSizer *topSizer = new wxBoxSizer(wxVERTICAL);

  wxStaticBoxSizer *contSizer =
    new wxStaticBoxSizer(new wxStaticBox(this, wxID_STATIC, "Contingency"),
			 wxVERTICAL);
  for (int pl = 1; pl <= p_cont.Length(); pl++) {
    NFPlayer *player = p_nfg.Players()[pl];
    wxString text;
    if (player->GetName() != "") {
      text += wxString::Format("%s: ", (const char *) player->GetName());
    }
    else {
      text += wxString::Format("Player %d: ", pl);
    }

    if (player->Strategies()[p_cont[pl]]->Name() != "") {
      text += (const char *) player->Strategies()[p_cont[pl]]->Name();
    }
    else {
      text += wxString::Format("Strategy %d", p_cont[pl]);
    }

    contSizer->Add(new wxStaticText(this, wxID_STATIC, text), 0, wxALL, 5);
  }

  topSizer->Add(contSizer, 0, wxALL | wxCENTER, 5);

  wxBoxSizer *outcomeSizer = new wxBoxSizer(wxHORIZONTAL);
  outcomeSizer->Add(new wxStaticText(this, wxID_STATIC, "Outcome"),
		    0, wxALL | wxCENTER, 5);
  m_outcome = new wxChoice(this, -1);
  m_outcome->Append("(null)");
  m_outcome->SetSelection(0);
  for (int outc = 1; outc <= p_nfg.NumOutcomes(); outc++) {
    NFOutcome *outcome = p_nfg.Outcomes()[outc];
    gText item = ToText(outc) + ": " + outcome->GetName();
    if (item == "") {
      item = "Outcome" + ToText(outc);
    }

    item += (" (" + ToText(p_nfg.Payoff(outcome, p_nfg.Players()[1])) + ", " +
	     ToText(p_nfg.Payoff(outcome, p_nfg.Players()[2])));
    if (p_nfg.NumPlayers() > 2) {
      item += ", " + ToText(p_nfg.Payoff(outcome, p_nfg.Players()[3]));
      if (p_nfg.NumPlayers() > 3) {
	item += ",...)";
      }
      else {
	item += ")";
      }
    }
    else {
      item += ")";
    }

    m_outcome->Append((const char *) item);
    if (p_nfg.GetOutcome(p_cont) == outcome) {
      m_outcome->SetSelection(outc);
    }
  }
  outcomeSizer->Add(m_outcome, 1, wxALL | wxEXPAND, 5);
  topSizer->Add(outcomeSizer, 0, wxALL | wxEXPAND, 5);

  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  wxButton *okButton = new wxButton(this, wxID_OK, "OK");
  okButton->SetDefault();
  buttonSizer->Add(okButton, 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
  buttonSizer->Add(new wxButton(this, wxID_HELP, "Help"), 0, wxALL, 5);
  topSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 5);

  SetSizer(topSizer);
  topSizer->Fit(this);
  topSizer->SetSizeHints(this);

  Layout();
}
