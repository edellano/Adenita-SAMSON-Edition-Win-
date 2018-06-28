#pragma once 

#include "SBDApp.hpp"
#include "SEAdenitaCoreSEAppGUI.hpp"
#include "ADNNanorobot.hpp"
#include "ADNSaveAndLoad.hpp"
#include "DASDaedalus.hpp"
#include "DASBackToTheAtom.hpp"
#include "DASCadnano.hpp"
#include "SEConfig.hpp"


/// This class implements the functionality of the app

// SAMSON Element generator pro tip: add non-GUI functionality in this class. The GUI functionality should go in the SEAdenitaCoreSEAppGUI class

class SEAdenitaCoreSEApp : public SBDApp {

	SB_CLASS

public :

	/// \name Constructors and destructors
	//@{

	SEAdenitaCoreSEApp();																													///< Constructs an app
	virtual ~SEAdenitaCoreSEApp();																											///< Destructs the app

	//@}

	/// \name GUI
	//@{

	SEAdenitaCoreSEAppGUI*											getGUI() const;															///< Returns a pointer to the GUI of the app

	//@}

  void LoadPart(QString filename);
  void SavePart(QString filename);
  void LoadPartWithDaedalus(QString filename, int minEdgeSize);
  void ImportFromCadnano(QString filename, ADNConstants::CadnanoLatticeType t);
  void ExportToOxDNA(QString folder, ADNAuxiliary::OxDNAOptions options);
  void SetScaffoldSequence(std::string seq);
  void CenterPart();

  ADNNanorobot* GetNanorobot();

private:
  void AddPartToActiveLayer(ADNPointer<ADNPart> part);

  ADNNanorobot* nanorobot_ = nullptr;
};


SB_REGISTER_TARGET_TYPE(SEAdenitaCoreSEApp, "SEAdenitaCoreSEApp", "85DB7CE6-AE36-0CF1-7195-4A5DF69B1528");
SB_DECLARE_BASE_TYPE(SEAdenitaCoreSEApp, SBDApp);
