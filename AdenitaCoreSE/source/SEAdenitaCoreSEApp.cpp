#include "SEAdenitaCoreSEApp.hpp"
#include "SEAdenitaCoreSEAppGUI.hpp"

SEAdenitaCoreSEApp::SEAdenitaCoreSEApp() {

	setGUI(new SEAdenitaCoreSEAppGUI(this));
	getGUI()->loadDefaultSettings();

  ADNLogger& logger = ADNLogger::GetLogger();
  SEConfig& config = SEConfig::GetInstance();
  if (config.clear_log_file) {
    logger.ClearLog();
  }
  logger.LogDateTime();

  nanorobot_ = new ADNNanorobot();

  //events
  /*SAMSON::getActiveDocument()->connectDocumentSignalToSlot(
    this,
    SB_SLOT(&SEAdenitaCoreSEApp::onDocumentEvent)
    );
*/
}

SEAdenitaCoreSEApp::~SEAdenitaCoreSEApp() {

	getGUI()->saveDefaultSettings();
	delete getGUI();

}

SEAdenitaCoreSEAppGUI* SEAdenitaCoreSEApp::getGUI() const { return static_cast<SEAdenitaCoreSEAppGUI*>(SBDApp::getGUI()); }

void SEAdenitaCoreSEApp::LoadPart(QString filename)
{
  ADNPointer<ADNPart> part = ADNLoader::LoadPartFromJson(filename.toStdString());

  AddPartToActiveLayer(part);
}

void SEAdenitaCoreSEApp::SaveFile(QString filename, bool all)
{
  if (all) {
    ADNLoader::SaveNanorobotToJson(nanorobot_, filename.toStdString());
  }
  else {
    auto parts = GetSelectedParts();
    auto part = parts[0];  // save first
    ADNLoader::SavePartToJson(part, filename.toStdString());
  }
}

void SEAdenitaCoreSEApp::LoadPartWithDaedalus(QString filename, int minEdgeSize)
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
  // Apply algorithm
  DASDaedalus *alg = new DASDaedalus();
  alg->SetMinEdgeLength(minEdgeSize);
  std::string seq = "";
  auto part = alg->ApplyAlgorithm(seq, filename.toStdString());

  AddPartToActiveLayer(part);
}

void SEAdenitaCoreSEApp::ImportFromCadnano(QString filename)
{
  DASCadnano cad = DASCadnano();
  ADNPointer<ADNPart> part = new ADNPart();
  std::string seq = "";

  part = cad.CreateCadnanoPart(filename.toStdString());
  
  AddPartToActiveLayer(part);
}

void SEAdenitaCoreSEApp::ExportToSequenceList(QString filename, bool all)
{
  // get selected part
  SBDocument* doc = SAMSON::getActiveDocument();
  SBNodeIndexer nodes;
  doc->getNodes(nodes, (SBNode::GetClass() == std::string("ADNPart")) && (SBNode::GetElementUUID() == SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));

  // only take one
  CollectionMap<ADNPart> parts;
  SB_FOR(SBNode* node, nodes) {
    if (node->isSelected()) {
      auto part = static_cast<ADNPart*>(node);
      parts.addReferenceTarget(part);
    }
  }

  if (parts.size() == 0 || all) {
    auto parts = nanorobot_->GetParts();
  }

  QFileInfo file = QFileInfo(filename);
  ADNLoader::OutputToCSV(parts, file.fileName().toStdString(), file.path().toStdString());
}

void SEAdenitaCoreSEApp::SetScaffoldSequence(std::string seq)
{
  std::string s = seq;
  if (s.empty()) {
    std::string filename = SB_ELEMENT_PATH + "/Data/m13mp18.fasta";
    std::vector<std::string> lines;
    SBIFileReader::getFileLines(filename, lines);
    for (unsigned int i = 1; i < lines.size(); i++) {
      std::string line = lines[i];
      if (line[0] != '>') {
        s.append(line);
      }
    }
  }

  // get selected part
  SBDocument* doc = SAMSON::getActiveDocument();
  SBNodeIndexer nodes;
  doc->getNodes(nodes, (SBNode::GetClass() == std::string("ADNPart")) && (SBNode::GetElementUUID() == SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));

  ADNPointer<ADNPart> part = nullptr;
  SB_FOR(SBNode* node, nodes) {
    if (node->isSelected()) {
      part = static_cast<ADNPart*>(node);
    }
  }

  if (part != nullptr) {
    auto scafs = part->GetScaffolds();
    SB_FOR(ADNPointer<ADNSingleStrand> ss, scafs) {
      ADNBasicOperations::SetSingleStrandSequence(ss, s);
    }
  }

}

void SEAdenitaCoreSEApp::ExportToOxDNA(QString folder, ADNAuxiliary::OxDNAOptions options, bool all)
{
  // get selected part
  SBDocument* doc = SAMSON::getActiveDocument();
  SBNodeIndexer nodes;
  doc->getNodes(nodes, (SBNode::GetClass() == std::string("ADNPart")) && (SBNode::GetElementUUID() == SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));

  // only take one
  ADNPointer<ADNPart> part = nullptr;
  SB_FOR(SBNode* node, nodes) {
    if (node->isSelected()) {
      part = static_cast<ADNPart*>(node);
    }
  }

  if (part == nullptr || all) {
    // nothing selected: export all
    ADNLoader::OutputToOxDNA(nanorobot_, folder.toStdString(), options);
  }
  else {
    ADNLoader::OutputToOxDNA(part, folder.toStdString(), options);
  }
}

void SEAdenitaCoreSEApp::CenterPart()
{
  // get selected part
  SBDocument* doc = SAMSON::getActiveDocument();
  SBNodeIndexer nodes;
  doc->getNodes(nodes, (SBNode::GetClass() == std::string("ADNPart")) && (SBNode::GetElementUUID() == SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));

  // only take one
  ADNPointer<ADNPart> part = nullptr;
  SB_FOR(SBNode* node, nodes) {
    if (node->isSelected()) {
      part = static_cast<ADNPart*>(node);
    }
  }

  if (part != nullptr) ADNBasicOperations::CenterPart(part);
}

void SEAdenitaCoreSEApp::ResetVisualModel() {
  //create visual model per nanorobot
  SBNodeIndexer allNodes;
  SAMSON::getActiveDocument()->getNodes(allNodes);
  ADNLogger& logger = ADNLogger::GetLogger();


  clock_t start = clock();

  bool vmAlreadyExist = false;
  SBPointer<SBVisualModel> vm;
  SB_FOR(SBNode* node, allNodes) {
    if (node->getType() == SBNode::VisualModel) {
      vm = static_cast<SBVisualModel*>(node);
      if (vm->getProxy()->getName() == "SEAdenitaVisualModel") {
        vmAlreadyExist = true;
        break;
      }
    }
  }
  
  if (vmAlreadyExist) {
    SBPointer<SEAdenitaVisualModel> adenitaVm = static_cast<SEAdenitaVisualModel*>(vm());
    adenitaVm->changeScale(6);
  }
  else {
    SBProxy* vmProxy = SAMSON::getProxy("SEAdenitaVisualModel");
    SEAdenitaVisualModel* adenitaVm = vmProxy->createInstance(allNodes);
    adenitaVm->create();
    SAMSON::getActiveLayer()->addChild(adenitaVm);
  }

  logger.LogPassedMilliseconds(start, "ResetVisualModel");

}

void SEAdenitaCoreSEApp::onDocumentEvent(SBDocumentEvent* documentEvent)
{
  ADNLogger& logger = ADNLogger::GetLogger();
  logger.Log(QString("document has been changed"));
}

void SEAdenitaCoreSEApp::onStructuralEvent(SBStructuralEvent* documentEvent)
{
  ADNLogger& logger = ADNLogger::GetLogger();
  logger.Log(QString("structure has been changed"));
  
  ResetVisualModel();
}

ADNNanorobot * SEAdenitaCoreSEApp::GetNanorobot()
{
  return nanorobot_;
}

CollectionMap<ADNPart> SEAdenitaCoreSEApp::GetSelectedParts()
{
  CollectionMap<ADNPart> parts;

  SBDocument* doc = SAMSON::getActiveDocument();
  SBNodeIndexer nodes;
  doc->getNodes(nodes, (SBNode::GetClass() == std::string("ADNPart")) && (SBNode::GetElementUUID() == SBUUID("DDA2A078-1AB6-96BA-0D14-EE1717632D7A")));

  // only take one
  SB_FOR(SBNode* node, nodes) {
    if (node->isSelected()) {
      ADNPointer<ADNPart> part = static_cast<ADNPart*>(node);
      parts.addReferenceTarget(part());
    }
  }

  return parts;
}

void SEAdenitaCoreSEApp::AddPartToActiveLayer(ADNPointer<ADNPart> part)
{
  DASBackToTheAtom btta = DASBackToTheAtom();
  btta.SetNucleotidesPostions(part);
  SEConfig& config = SEConfig::GetInstance();
  if (config.use_atomic_details) {
    btta.GenerateAllAtomModel(part);
  }
  btta.CheckDistances(part);

  nanorobot_->RegisterPart(part);

  //events
  ConnectStructuralSignalSlots(part);

  SAMSON::beginHolding("Add model");
  part->create();
  SAMSON::getActiveLayer()->addChild(part());
  SAMSON::endHolding();
}

void SEAdenitaCoreSEApp::ConnectStructuralSignalSlots(ADNPointer<ADNPart> part)
{
  auto singleStrands = part->GetSingleStrands();

  part->connectStructuralSignalToSlot(
    this,
    SB_SLOT(&SEAdenitaCoreSEApp::onStructuralEvent)
    );

  //SB_FOR(auto singleStrand, singleStrands) {
  //  auto nucleotides = singleStrand->GetNucleotides();

  //  singleStrand->connectStructuralSignalToSlot(
  //    this,
  //    SB_SLOT(&SEAdenitaCoreSEApp::onStructuralEvent)
  //    );

  //  SB_FOR(auto nucleotide, nucleotides) {
  //    nucleotide->connectStructuralSignalToSlot(
  //      this,
  //      SB_SLOT(&SEAdenitaCoreSEApp::onStructuralEvent)
  //      );
  //  }
  //}

  ////todo connect double strands signals also to slots
  //auto doubleStrands = nanorobot_->GetDoubleStrands(part);
  


}
