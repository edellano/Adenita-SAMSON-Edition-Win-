#pragma once 

#include "SBMVisualModel.hpp"
#include "SBGApp.hpp" 
#include "SEAdenitaCoreSEApp.hpp"
#include "ADNDisplayHelper.hpp"

#include "SBBaseEvent.hpp"
#include "SBDocumentEvent.hpp"
#include "SBStructuralEvent.hpp"
#include "ADNAuxiliary.hpp"
#include "ADNArray.hpp"
#include "ADNMixins.hpp"
#include "ADNPart.hpp"
#include "ADNNanorobot.hpp"
#include "SEConfig.hpp"
#include "MSVColors.hpp"
#include "PIPrimer3.hpp"
#include <QOpenGLFunctions_4_3_Core>


/// This class implements a visual model

class SEAdenitaVisualModel : public SBMVisualModel {

	SB_CLASS

public :

  double const MAX_SCALE = 4.0;

  enum Scale {
    ATOMS_STICKS = 0,
    ATOMS_BALLS = 1,
    NUCLEOTIDES = 2,
    SINGLE_STRANDS = 3,
    DOUBLE_STRANDS = 4
  };

	/// \name Constructors and destructors
	//@{

	SEAdenitaVisualModel();																													///< Builds a visual model					
	SEAdenitaVisualModel(const SBNodeIndexer& nodeIndexer);																					///< Builds a visual model 
	virtual ~SEAdenitaVisualModel();																											///< Destructs the visual model

	//@}

	/// \name Serialization
	//@{

	virtual bool												isSerializable() const;													///< Returns true when the class is serializable

	virtual void												serialize(SBCSerializer* serializer, const SBNodeIndexer& nodeIndexer, const SBVersionNumber& sdkVersionNumber = SB_SDK_VERSION_NUMBER, const SBVersionNumber& classVersionNumber = SBVersionNumber(1, 0, 0)) const;		///< Serializes the node
	virtual void												unserialize(SBCSerializer* serializer, const SBNodeIndexer& nodeIndexer, const SBVersionNumber& sdkVersionNumber = SB_SDK_VERSION_NUMBER, const SBVersionNumber& classVersionNumber = SBVersionNumber(1, 0, 0));			///< Unserializes the node

	//@}

	/// \name Topology
	//@{

	virtual	void												eraseImplementation();													///< Erases the visual model

	//@}

	/// \name Rendering
	//@{
  float       												getScale();																
  void												        changeScaleDiscrete(int scale, bool createIndex = true);																///< Displays the visual model
  void												        changeScale(double scale, bool createIndex = true);																///< Displays the visual model
  void												        changeDimension(int dimension);																///< Displays the visual model
  void												        changeVisibility(double layer);																///< Displays the visual model
  void                                changePropertyColors(int propertyIdx, int colorSchemeIdx);
  void                                changeHighlight(int highlightIdx);
  void                                setSingleStrandColors(int index);
  void                                setNucleotideColors(int index);
  void                                setDoubleStrandColors(int index);
  void                                update();

  virtual void												display();																///< Displays the visual model
  virtual void												displayForShadow();														///< Displays the visual model for shadow purposes
	virtual void												displayForSelection();													///< Displays the visual model for selection purposes

  virtual void                        highlightNucleotides();

	virtual void												expandBounds(SBIAPosition3& bounds) const;								///< Expands the bounds to make sure the visual model fits inside them

	virtual void												collectAmbientOcclusion(const SBPosition3& boxOrigin, const SBPosition3& boxSize, unsigned int nCellsX, unsigned int nCellsY, unsigned int nCellsZ, float* ambientOcclusionData);		///< To collect ambient occlusion data

	//@}

	/// \name Events
	//@{

	virtual void												onBaseEvent(SBBaseEvent* baseEvent);									///< Handles base events
	virtual void												onDocumentEvent(SBDocumentEvent* documentEvent);						///< Handles document events
	virtual void												onStructuralEvent(SBStructuralEvent* documentEvent);					///< Handles structural events
  void                                showBasePairing(bool show);

	//@}

private:
  void                                init();
  void												        initNucleotidesAndSingleStrands(bool createIndex = true);
  void												        initDoubleStrands(bool createIndex = true);
  ADNArray<unsigned int>              getNucleotideIndices();
  ADNArray<unsigned int>              getBaseSegmentIndices();
  void												        highlightFlagChanged(); //scale 9: display polyhedron 
  SEAdenitaCoreSEApp*					        getAdenitaApp() const;															///< Returns a pointer to the app
  void                                orderVisibility();
  void                                setupPropertyColors();
  ADNArray<float>                     calcPropertyColor(int colorSchemeIdx, float min, float max, float val);
  void                                displayLoops(ADNNucleotide *nt, unsigned int index);
  void                                displayBasePairConnections(bool onlySelected);
  void                                displayForDebugging();
  void                                displayCircularDNAConnection();
  void                                displayTags();
  void                                updateStructuralEvents();

  void												        prepareNucleotides();
  void												        prepareSingleStrands();
  void												        prepareDoubleStrands();
  void												        displayNucleotides(bool forSelection = false);
  void												        displaySingleStrands(bool forSelection = false);
  void												        displayDoubleStrands(bool forSelection = false);
  void												        prepareArraysTransition(); // Prepare the arrays for displaying (this separates the interpolation from display)
  void												        displayNoTransition(bool forSelection = false);
  void												        prepareArraysNoTranstion();
  void												        displayTransition(); 
  void												        prepareSticksToBalls(double iv, bool forSelection = false);
  void												        prepareSingleStrandsToDoubleStrands(double iv, bool forSelection = false);


  // general display properties 
  ADNArray<float> nucleotideEColor_;
  
  double scale_ = 3;
  int dim_ = 3;

  ADNNanorobot * nanorobot_;

  //nucleotide scale
  unsigned int nPositionsNt_;
  unsigned int nCylindersNt_;
  ADNArray<float> colorsVNt_;
  ADNArray<float> colorsENt_;
  ADNArray<float> positionsNt_;
  ADNArray<float> radiiVNt_;
  ADNArray<float> radiiENt_;
  ADNArray<unsigned int> flagsNt_;
  ADNArray<unsigned int> nodeIndicesNt_;
  ADNArray<unsigned int> indicesNt_;

  //single strand scale
  ADNArray<float> colorsVSS_;
  ADNArray<float> colorsESS_;
  ADNArray<float> radiiVSS_;
  ADNArray<float> radiiESS_;

  //double strand scale
  unsigned int nPositionsDS_;
  unsigned int nCylindersDS_;
  ADNArray<float> colorsVDS_;
  ADNArray<float> positionsDS_;
  ADNArray<float> radiiVDS_;
  ADNArray<unsigned int> flagsDS_;
  ADNArray<unsigned int> nodeIndicesDS_;

  std::map<ADNNucleotide*, unsigned int> ntMap_;
  std::map<ADNBaseSegment*, unsigned int> bsMap_;

  map<ADNNucleotide*, float> sortedNucleotidesByDist_;
  map<ADNSingleStrand*, float> sortedSingleStrandsByDist_;
 
  // current arrays for being displayed (only spheres and cylinders)

  vector<ADNArray<float>> propertyColorSchemes_;
  
  enum ColorType {
    REGULAR = 0,  // default color map
    MELTTEMP = 1,  // melting temperatures color map
    GIBBS = 2  // gibbs free energy color map
  };

  ColorType curColorType_ = REGULAR;

  std::map<ColorType, MSVColors*> colors_;

  enum HighlightType {
    NONE = 0,
    CROSSOVERS = 1,
    GC = 2,
    TAGGED
  };

  HighlightType highlightType_ = NONE;
  bool showBasePairing_ = false;
};


SB_REGISTER_TARGET_TYPE(SEAdenitaVisualModel, "SEAdenitaVisualModel", "315CE7EA-009E-CE93-E00C-E921865953AB");
SB_DECLARE_BASE_TYPE(SEAdenitaVisualModel, SBMVisualModel);
