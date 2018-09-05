#pragma once 

#include "SBGEditor.hpp"
#include "SENanotubeCreatorEditorGUI.hpp"

#include "SBBaseEvent.hpp"
#include "SBDocumentEvent.hpp"
#include "SBDynamicalEvent.hpp"
#include "SBStructuralEvent.hpp"
#include "SBAction.hpp"
#include "ADNPart.hpp"
#include "SEAdenitaCoreSEApp.hpp"
#include "ADNDisplayHelper.hpp"
#include "DASCreator.hpp"


enum RoutingType {
  None = 0,
  Seamless = 1,
  Tiles = 2,
  NonSeamless = 3
};

/// This class implements an editor

class SENanotubeCreatorEditor : public SBGEditor {

	SB_CLASS
	Q_OBJECT

public :

	/// \name Constructors and destructors
	//@{

	SENanotubeCreatorEditor();																													///< Builds an editor					
	virtual ~SENanotubeCreatorEditor();																											///< Destructs the editor

	//@}

	/// \name Identity
	//@{

	virtual SBCContainerUUID									getUUID() const;														///< Returns the widget UUID
	virtual QString												getName() const;														///< Returns the class name
	virtual QString												getText() const;														///< Returns the menu item text
	virtual QPixmap												getLogo() const;														///< Returns the pixmap logo
	virtual QKeySequence										getShortcut() const;													///< Returns the shorcut
	virtual QString												getToolTip() const;														///< Returns the tool tip

	//@}

	/// \name Editing
	//@{

	virtual void												beginEditing();															///< Called when editing is about to begin
	virtual void												endEditing();															///< Called when editing is about to end

	//@}

	/// \name Actions
	//@{

	virtual void												getActions(SBVector<SBAction*>& actionVector);							///< Gets the editor's actions

	//@}

	/// \name Rendering
	//@{

	virtual void												display();																///< Displays the editor
	virtual void												displayForShadow();														///< Displays the editor for shadow purposes
	virtual void												displayInterface();														///< Displays the 2D interface in the viewport

	//@}

	/// \name GUI Events
	//@{

	virtual void												mousePressEvent(QMouseEvent* event);									///< Handles mouse press event
	virtual void												mouseReleaseEvent(QMouseEvent* event);									///< Handles mouse release event
	virtual void												mouseMoveEvent(QMouseEvent* event);										///< Handles mouse move event
	virtual void												mouseDoubleClickEvent(QMouseEvent* event);								///< Handles mouse double click event

	virtual void												wheelEvent(QWheelEvent* event);											///< Handles wheel event

	virtual void												keyPressEvent(QKeyEvent* event);										///< Handles key press event
	virtual void												keyReleaseEvent(QKeyEvent* event);										///< Handles key release event

	//@}

	/// \name SAMSON Events
	//@{

	virtual void												onBaseEvent(SBBaseEvent* baseEvent);									///< Handles base events
	virtual void												onDynamicalEvent(SBDynamicalEvent* dynamicalEvent);						///< Handles dynamical events
	virtual void												onDocumentEvent(SBDocumentEvent* documentEvent);						///< Handles document events
	virtual void												onStructuralEvent(SBStructuralEvent* documentEvent);					///< Handles structural events

	//@}

	/// \name GUI
	//@{

	SENanotubeCreatorEditorGUI*											getPropertyWidget() const;												///< Returns the property widget of the editor

	//@}

  void SetRouting(RoutingType t);
private:

  ADNPointer<ADNPart> generateNanotube(bool mock = false);
  void displayNanotube();
  void resetPositions();
  void sendPartToAdenita(ADNPointer<ADNPart> nanotube);

  struct Positions {
    SBPosition3 First;
    SBPosition3 Second;
    SBPosition3 Third;
    SBPosition3 Fourth;
    SBPosition3 Fifth;
    SBPosition3 Sixth;
    int cnt = 0;
  };

  Positions positions_;
  bool display_ = false;
  ADNPointer<ADNPart> tempPart_ = nullptr;
  RoutingType routing_ = RoutingType::None;
};


SB_REGISTER_TYPE(SENanotubeCreatorEditor, "SENanotubeCreatorEditor", "4B6A0B18-48B5-233A-28A4-BA3EF3D56AB8");
SB_DECLARE_BASE_TYPE(SENanotubeCreatorEditor, SBGEditor);
