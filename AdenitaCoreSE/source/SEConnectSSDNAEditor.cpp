#include "SEConnectSSDNAEditor.hpp"
#include "SAMSON.hpp"


SEConnectSSDNAEditor::SEConnectSSDNAEditor() {

	// SAMSON Element generator pro tip: this default constructor is called when unserializing the node, so it should perform all default initializations.

	propertyWidget = new SEConnectSSDNAEditorGUI(this);
	propertyWidget->loadDefaultSettings();
	SAMSON::addWidget(propertyWidget);

}

SEConnectSSDNAEditor::~SEConnectSSDNAEditor() {

	// SAMSON Element generator pro tip: disconnect from signals you might have connected to.

	propertyWidget->saveDefaultSettings();
	delete propertyWidget;

}

SEConnectSSDNAEditorGUI* SEConnectSSDNAEditor::getPropertyWidget() const { return static_cast<SEConnectSSDNAEditorGUI*>(propertyWidget); }

SEAdenitaCoreSEApp* SEConnectSSDNAEditor::getAdenitaApp() const
{
  return static_cast<SEAdenitaCoreSEApp*>(SAMSON::getApp(SBCContainerUUID("85DB7CE6-AE36-0CF1-7195-4A5DF69B1528"), SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));
}

SBCContainerUUID SEConnectSSDNAEditor::getUUID() const { return SBCContainerUUID("0854A585-E146-954F-616C-B4532A1B2555"); }

QString SEConnectSSDNAEditor::getName() const { 

	// SAMSON Element generator pro tip: this name should not be changed

	return "SEConnectSSDNAEditor"; 

}

QString SEConnectSSDNAEditor::getText() const { 
	
	// SAMSON Element generator pro tip: modify this function to return a user-friendly string that will be displayed in menus

	return QObject::tr("SEConnectSSDNAEditor"); 

}

QPixmap SEConnectSSDNAEditor::getLogo() const {

	// SAMSON Element generator pro tip: this icon will be visible in the GUI title bar. 
	// Modify it to better reflect the purpose of your editor.

	return QPixmap(QString::fromStdString(SB_ELEMENT_PATH + "/Resource/Icons/connectSS.png"));

}

QKeySequence SEConnectSSDNAEditor::getShortcut() const { 
	
	// SAMSON Element generator pro tip: modify this function to associate a tentative shortcut to your editor

	return QKeySequence(""); 

}

QString SEConnectSSDNAEditor::getToolTip() const { 
	
	// SAMSON Element generator pro tip: modify this function to have your editor display a tool tip in the SAMSON GUI when the mouse hovers the editor's icon

	return QObject::tr("SAMSON Element generator pro tip: modify me"); 

}

void SEConnectSSDNAEditor::beginEditing() {

	// SAMSON Element generator pro tip: SAMSON calls this function when your editor becomes active. 
	// Implement this function if you need to prepare some data structures in order to be able to handle GUI or SAMSON events.

}

void SEConnectSSDNAEditor::endEditing() {

	// SAMSON Element generator pro tip: SAMSON calls this function immediately before your editor becomes inactive (for example when another editor becomes active). 
	// Implement this function if you need to clean some data structures.

}

void SEConnectSSDNAEditor::getActions(SBVector<SBAction*>& actionVector) {

	// SAMSON Element generator pro tip: SAMSON calls this function to show the user actions associated to your editor in context menus.
	// Append actions to the actionVector if necessary.
	// Please refer to tutorials for examples.

}

void SEConnectSSDNAEditor::display() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop. 
	// Implement this function to display things in SAMSON, for example thanks to the utility functions provided by SAMSON (e.g. displaySpheres, displayTriangles, etc.)

  if (display_) {
    SBPosition3 currentPosition = SAMSON::getWorldPositionFromViewportPosition(SAMSON::getMousePositionInViewport());
    
    //check if a nucleotide got selected
    auto app = getAdenitaApp();
    auto nanorobot = app->GetNanorobot();

    auto highlightedNucleotides = nanorobot->GetHighlightedNucleotides();
    
    if (highlightedNucleotides.size() == 1) {
      currentPosition = highlightedNucleotides[0]->GetBackbone()->GetPosition();
    }
    
    ADNDisplayHelper::displayCylinder(start_->GetBackbone()->GetPosition(), currentPosition);
  }

}

void SEConnectSSDNAEditor::displayForShadow() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop in order to compute shadows. 
	// Implement this function if your editor displays things in viewports, so that your editor can cast shadows
	// to other objects in SAMSON, for example thanks to the utility
	// functions provided by SAMSON (e.g. displaySpheres, displayTriangles, etc.)

}

void SEConnectSSDNAEditor::displayInterface() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop in order to display the editor 2D interface in viewports. 
	// Implement this function if your editor displays a 2D user interface. For example, a rectangle selection editor would display a 2D rectangle in the active viewport. 
	// You may use utility functions provided by SAMSON (e.g. displayLinesOrtho and displayTrianglesOrtho).

}

void SEConnectSSDNAEditor::mousePressEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.
  if (event->buttons() == Qt::LeftButton && !display_) {

    //check if a nucleotide got selected
    auto app = getAdenitaApp();
    auto nanorobot = app->GetNanorobot();

    auto selectedNucleotides = nanorobot->GetSelectedNucleotides();
    auto highlightedNucleotides = nanorobot->GetHighlightedNucleotides();

    SB_FOR(auto node, selectedNucleotides) {
      node->setSelectionFlag(false);
    }

    if (highlightedNucleotides.size() == 1) {
      auto nt = highlightedNucleotides[0];
      
      nt->setSelectionFlag(true);
      start_ = nt;
      display_ = true;

    }

  }
}

void SEConnectSSDNAEditor::mouseReleaseEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.
  
  if (display_) {
    display_ = false;

    auto app = getAdenitaApp();
    auto nanorobot = app->GetNanorobot();

    auto highlightedNucleotides = nanorobot->GetHighlightedNucleotides();
    
    if (highlightedNucleotides.size() == 1) {
      auto end = highlightedNucleotides[0];
      bool start53 = start_->GetEnd() == FivePrime || start_->GetEnd() == ThreePrime;
      bool end53 = end->GetEnd() == FivePrime || end->GetEnd() == ThreePrime;
      if (start53 && end53) {
        end->setSelectionFlag(true);
        app->ConnectSingleStrands();
      }
      else { //if not the respective endings are selected, then break at the locations and connect the new endings
        auto selectedNucleotides = nanorobot->GetSelectedNucleotides();
        
        SB_FOR(auto node, selectedNucleotides) {
          node->setSelectionFlag(false);
        }

        //break the strands first
        if (start53 && !end53) {
          end->setSelectionFlag(true);
          app->BreakSingleStrand();
        }
        else if (!start53 && end53) {
          start_->setSelectionFlag(true);
          app->BreakSingleStrand();
        }
        else {
          start_->setSelectionFlag(true);
          app->BreakSingleStrand();
          start_->setSelectionFlag(false);
          end->setSelectionFlag(true);
          app->BreakSingleStrand();
          end->setSelectionFlag(false);
        }

        //connect at broken location
        start_->setSelectionFlag(true);
        end->setSelectionFlag(true);
        app->ConnectSingleStrands();
      }


      

    }
  }

}

void SEConnectSSDNAEditor::mouseMoveEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.
  if (display_) {
    SAMSON::requestViewportUpdate();
  }
}

void SEConnectSSDNAEditor::mouseDoubleClickEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEConnectSSDNAEditor::wheelEvent(QWheelEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEConnectSSDNAEditor::keyPressEvent(QKeyEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEConnectSSDNAEditor::keyReleaseEvent(QKeyEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEConnectSSDNAEditor::onBaseEvent(SBBaseEvent* baseEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle base events

}

void SEConnectSSDNAEditor::onDocumentEvent(SBDocumentEvent* documentEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle document events 

}

void SEConnectSSDNAEditor::onDynamicalEvent(SBDynamicalEvent* dynamicalEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle dynamical events 

}

void SEConnectSSDNAEditor::onStructuralEvent(SBStructuralEvent* documentEvent) {
	
	// SAMSON Element generator pro tip: implement this function if you need to handle structural events

}