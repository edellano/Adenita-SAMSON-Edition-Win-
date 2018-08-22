#include "DASCreator.hpp"

//ADNPart * DASEditor::CreateTwoTubes(size_t length, SBPosition3 start, SBVector3 direction, SBVector3 sepDir) {
//  ADNPart* part = new ADNPart();
//
//  // direction in which the double helices would be separated
//  //SBVector3 sepDir = SBVector3(direction[1], -direction[0], SBQuantity::dimensionless(0.0));
//  SBQuantity::length sepDist = SBQuantity::angstrom(0.5*ADNConstants::DH_DIST + 5*ADNConstants::DH_DIAMETER);
//  SBPosition3 startFirst = start - sepDist * sepDir;
//  SBPosition3 startSecond = start + sepDist * sepDir + SBQuantity::nanometer(length * ADNConstants::BP_RISE) * direction;
//
//  AddDoubleStrandToANTPart(part, length, startFirst, direction);
//  AddDoubleStrandToANTPart(part, length, startSecond, -direction);
//
//  part->SetScaffold(0);
//  part->SetNtSegmentMap();
//  return part;
//}
//

//ANTPart * DASEditor::CreateCrippledWireframeCuboid(SBPosition3 topLeftFront, int xSize, int ySize, int zSize) {
//  ANTPart* part = new ANTPart();
//
//  SBVector3 xDir(1.0, 0.0, 0.0);
//  SBVector3 yDir(0.0, 1.0, 0.0);
//  SBVector3 zDir(0.0, 0.0, 1.0);
//
//  SBQuantity::nanometer xLength = SBQuantity::nanometer(ANTConstants::BP_RISE * xSize);
//  SBQuantity::nanometer yLength = SBQuantity::nanometer(ANTConstants::BP_RISE * ySize);
//  SBQuantity::nanometer zLength = SBQuantity::nanometer(ANTConstants::BP_RISE * zSize);
//
//  // create 12 double helices
//  AddCrippledDoubleStrandToANTPart(topLeftFront, xDir, xSize, *part);
//  AddCrippledDoubleStrandToANTPart(topLeftFront, -yDir, ySize, *part);
//  AddCrippledDoubleStrandToANTPart(topLeftFront, -zDir, zSize, *part);
//
//  SBPosition3 bottomRightFront = topLeftFront + (xLength*xDir - yLength*yDir);
//
//  AddCrippledDoubleStrandToANTPart(bottomRightFront, -xDir, xSize, *part);
//  AddCrippledDoubleStrandToANTPart(bottomRightFront, yDir, ySize, *part);
//  AddCrippledDoubleStrandToANTPart(bottomRightFront, -zDir, zSize, *part);
//
//  SBPosition3 bottomLeftBack = topLeftFront - (yLength*yDir + zLength*zDir);
//
//  AddCrippledDoubleStrandToANTPart(bottomLeftBack, xDir, xSize, *part);
//  AddCrippledDoubleStrandToANTPart(bottomLeftBack, yDir, ySize, *part);
//  AddCrippledDoubleStrandToANTPart(bottomLeftBack, zDir, zSize, *part);
//
//  SBPosition3 topRightBack = topLeftFront + (xLength*xDir - zLength*zDir);
//
//  AddCrippledDoubleStrandToANTPart(topRightBack, -xDir, xSize, *part);
//  AddCrippledDoubleStrandToANTPart(topRightBack, -yDir, ySize, *part);
//  AddCrippledDoubleStrandToANTPart(topRightBack, zDir, zSize, *part);
//
//  return part;
//}
//
//ANTPart * DASEditor::DebugCrossoversPart() {
//  ANTPart* part = new ANTPart();
//
//  SBPosition3 init_pos = SBPosition3();
//  SBVector3 init_dir = SBVector3(0.0, 0.0, 1.0);
//  size_t length = 42;
//
//  AddDoubleStrandToANTPart(part, length, init_pos, init_dir);
//
//  SBVector3 dirPlane = SBVector3(1.0, 0.0, 1.0);
//  SBPosition3 pos = init_pos + SBQuantity::nanometer(ANTConstants::DH_DIAMETER + 0.25) * dirPlane;
//  SBPosition3 posFinal = pos + SBQuantity::nanometer(ANTConstants::BP_RISE * length) * init_dir;
//
//  AddDoubleStrandToANTPart(part, length, posFinal, -init_dir);
//  AddDoubleStrandToANTPart(part, length, -pos, init_dir);
//
//  part->SetScaffold(0);
//  part->SetNtSegmentMap();
//
//  return part;
//}

// todo: calculate positions
ADNPointer<ADNDoubleStrand> DASCreator::CreateDoubleStrand(ADNPointer<ADNPart> part, int length, SBPosition3 start, SBVector3 direction)
{
  return AddDoubleStrandToADNPart(part, length, start, direction);
}

ADNPointer<ADNSingleStrand> DASCreator::CreateSingleStrand(ADNPointer<ADNPart> part, int length, SBPosition3 start, SBVector3 direction)
{
  return AddSingleStrandToADNPart(part, length, start, direction);
}

ADNPointer<ADNLoop> DASCreator::CreateLoop(ADNPointer<ADNSingleStrand> ss, ADNPointer<ADNNucleotide> nextNt, std::string seq, ADNPointer<ADNPart> part)
{
  ADNPointer<ADNLoop> loop = new ADNLoop();

  for (size_t k = 0; k < seq.size(); ++k) {
    ADNPointer<ADNNucleotide> nt = new ADNNucleotide();
    if (part != nullptr) {
      part->RegisterNucleotide(ss, nt, nextNt);
    }else {
      ss->AddNucleotide(nt, nextNt);
    }
    nt->SetType(ADNModel::ResidueNameToType(seq[k]));
    loop->AddNucleotide(nt);
    if (k == 0) loop->SetStart(nt);
    if (k == seq.size() - 1) loop->SetEnd(nt);
  }

  return loop;
}

ADNPointer<ADNPart> DASCreator::CreateNanotube(SBQuantity::length radius, SBPosition3 center, SBVector3 direction, int length, bool mock)
{
  int minHeight = 1;
  int minNanotubes = 3;

  ADNPointer<ADNPart> nanorobot = nullptr;
  
  // so far create only a circle
  // transformation to global coordinates
  ublas::vector<double> dir = ublas::vector<double>(3);
  dir[0] = direction[0].getValue();
  dir[1] = direction[1].getValue();
  dir[2] = direction[2].getValue();
  ublas::matrix<double> subspace = ADNVectorMath::FindOrthogonalSubspace(dir);
  ADNVectorMath::AddRowToMatrix(subspace, dir);
  
  if (length < minHeight) {
    length = minHeight;
  }

  if (radius > SBQuantity::length(0.0)) {
    // number of double helices that fit into the circumpherence
    auto diameter = 2 * radius;

    SBQuantity::length r = SBQuantity::nanometer(ADNConstants::DH_DIAMETER * 0.5);
    SBQuantity::length R = diameter * 0.5;
  
    // in a circumpherence every double strand is going to take up the same space
    auto up = -r*r*(4 * R*R - r*r) + (2 * R*R - r*r)*(2 * R*R - r*r);
    auto down = r*r*(4 * R*R - r*r) + (2 * R*R - r*r)*(2 * R*R - r*r);
    auto cosTheta = up / down;
    auto theta = acos(cosTheta.getValue());
    double pi = atan(1.0) * 4.0;
    int num = ceil(2 * pi / theta);

    if (num < minNanotubes) {
      num = minNanotubes;
      theta = ADNVectorMath::DegToRad(120.0);
    }

    // recalculate the exact radius so num will fit
    auto newR = theta * num * R / (2 * pi);
    auto newTheta = 2 * pi / num;

    if (num > 0) {
      nanorobot = new ADNPart();
      // create dsDNA
      double t = 0.0;
      for (int j = 0; j < num; ++j) {
        //  // a and b are the coordinates on the plane
        auto a = newR*sin(t);
        auto b = newR*cos(t);
        ublas::vector<double> pos_p(3);
        pos_p[0] = a.getValue();
        pos_p[1] = b.getValue();
        pos_p[2] = 0.0;
        ublas::vector<double> trf = ublas::prod(ublas::trans(subspace), pos_p);
        SBPosition3 dsPosition = SBPosition3(SBQuantity::picometer(trf[0]), SBQuantity::picometer(trf[1]), SBQuantity::picometer(trf[2])) + center;
        if (mock) {
          AddMockDoubleStrandToADNPart(nanorobot, length, dsPosition, direction);
        }
        else {
          AddDoubleStrandToADNPart(nanorobot, length, dsPosition, direction);
        }
        t += newTheta;
      }

      nanorobot->SetE1(ADNVectorMath::row(subspace, 0));
      nanorobot->SetE2(ADNVectorMath::row(subspace, 1));
      nanorobot->SetE3(ADNVectorMath::row(subspace, 2));
    }

    ADNLogger& logger = ADNLogger::GetLogger();
    logger.LogDebugDateTime();
    logger.LogDebug(std::string("-> Creating DNA nanotube"));
    logger.LogDebug(std::string("    * num of ds: ") + std::to_string(num));
    logger.LogDebug(std::string("    * bps per ds: ") + std::to_string(length));
    logger.LogDebug(std::string("    * total bps: ") + std::to_string(length*num));
  }

  return nanorobot;
}

ADNPointer<ADNPart> DASCreator::CreateMockNanotube(SBQuantity::length radius, SBPosition3 center, SBVector3 direction, int length)
{
  return CreateNanotube(radius, center, direction, length, true);
}

ADNPointer<ADNDoubleStrand> DASCreator::AddDoubleStrandToADNPart(ADNPointer<ADNPart> part, size_t length, SBPosition3 start, SBVector3 direction)
{
  SBPosition3 delt = SBQuantity::nanometer(ADNConstants::BP_RISE) * direction;
  SBPosition3 pos = start;

  ADNPointer<ADNDoubleStrand> ds = new ADNDoubleStrand();
  part->RegisterDoubleStrand(ds);

  ADNPointer<ADNSingleStrand> ssLeft = new ADNSingleStrand();
  part->RegisterSingleStrand(ssLeft);

  ADNPointer<ADNSingleStrand> ssRight = new ADNSingleStrand();
  part->RegisterSingleStrand(ssRight);

  for (size_t i = 0; i < length; ++i) {
    ADNPointer<ADNBaseSegment> bs = new ADNBaseSegment();

    bs->SetPosition(pos);
    bs->SetE3(ADNAuxiliary::SBVectorToUblasVector(direction));
    bs->SetNumber(boost::numeric_cast<int>(i));

    ADNPointer<ADNBasePair> cell = new ADNBasePair();
    // create nucleotides
    ADNPointer<ADNNucleotide> ntLeft = new ADNNucleotide();
    part->RegisterNucleotideThreePrime(ssLeft, ntLeft);
    cell->SetLeftNucleotide(ntLeft);
    ntLeft->SetPosition(bs->GetPosition());
    ntLeft->SetBackbonePosition(bs->GetPosition());
    ntLeft->SetSidechainPosition(bs->GetPosition());
    ntLeft->SetBaseSegment(bs);
    ntLeft->SetType(DNABlocks::DI);

    ADNPointer<ADNNucleotide> ntRight = new ADNNucleotide();
    part->RegisterNucleotideFivePrime(ssRight, ntRight);
    cell->SetRightNucleotide(ntRight);
    ntRight->SetPosition(bs->GetPosition());
    ntRight->SetBackbonePosition(bs->GetPosition());
    ntRight->SetSidechainPosition(bs->GetPosition());
    ntRight->SetBaseSegment(bs);
    ntRight->SetType(DNABlocks::DI);

    ntLeft->SetPair(ntRight);
    ntRight->SetPair(ntLeft);

    bs->SetCell(cell());

    part->RegisterBaseSegmentEnd(ds, bs);

    pos += delt;
  }

  ssLeft->SetDefaultName();
  ssRight->SetDefaultName();

  return ds;
}

ADNPointer<ADNDoubleStrand> DASCreator::AddMockDoubleStrandToADNPart(ADNPointer<ADNPart> part, size_t length, SBPosition3 start, SBVector3 direction)
{
  SBPosition3 delt = SBQuantity::nanometer(ADNConstants::BP_RISE) * direction;
  SBPosition3 pos = start;

  ADNPointer<ADNDoubleStrand> ds = new ADNDoubleStrand();
  part->RegisterDoubleStrand(ds);

  for (size_t i = 0; i < length; ++i) {
    ADNPointer<ADNBaseSegment> bs = new ADNBaseSegment();

    bs->SetPosition(pos);
    bs->SetE3(ADNAuxiliary::SBVectorToUblasVector(direction));
    bs->SetNumber(boost::numeric_cast<int>(i));

    part->RegisterBaseSegmentEnd(ds, bs);

    pos += delt;
  }

  return ds;
}

ADNPointer<ADNSingleStrand> DASCreator::AddSingleStrandToADNPart(ADNPointer<ADNPart> part, size_t length, SBPosition3 start, SBVector3 direction)
{
  SBPosition3 delt = SBQuantity::nanometer(ADNConstants::BP_RISE) * direction;
  SBPosition3 pos = start;

  ADNPointer<ADNDoubleStrand> ds = new ADNDoubleStrand();

  // create nucleotides
  ADNPointer<ADNSingleStrand> ss = new ADNSingleStrand();

  for (size_t i = 0; i < length; ++i) {
    ADNPointer<ADNBaseSegment> bs = new ADNBaseSegment();
    ds->AddBaseSegmentEnd(bs);

    ADNPointer<ADNBasePair> bp = new ADNBasePair();
    bs->SetCell(bp());
    bs->SetPosition(pos);

    ADNPointer<ADNNucleotide> nt = new ADNNucleotide();
    nt->SetType(DNABlocks::DI);
    nt->SetPosition(bs->GetPosition());
    nt->SetBackbonePosition(bs->GetPosition());
    nt->SetSidechainPosition(bs->GetPosition());
    nt->SetBaseSegment(bs);
    bp->SetLeftNucleotide(nt);

    ss->AddNucleotideThreePrime(nt);

    pos += delt;
  }

  ss->SetDefaultName();

  return ss;
}