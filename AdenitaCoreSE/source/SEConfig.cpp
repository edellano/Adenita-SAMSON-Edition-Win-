#include "SEConfig.hpp"

SEConfig & SEConfig::GetInstance()
{
  static SEConfig instance;
  return instance;
}

SEConfig::SEConfig() {
  loadConfig();
}

void SEConfig::loadConfig() {
  QFileInfo check_file(DEFAULT_CONFIGPATH.c_str());

  if (!(check_file.exists() && check_file.isFile())) {
    StringBuffer s;
    Writer<StringBuffer> writer(s);

    writer.StartObject();

    writer.Key("min_melting_temp");
    writer.Double(min_melting_temp);

    writer.Key("max_melting_temp");
    writer.Double(max_melting_temp);

    writer.Key("min_gibbs_free_energy");
    writer.Double(min_gibbs_free_energy);

    writer.Key("interpolate_dimensions");
    writer.Bool(interpolate_dimensions);

    writer.Key("animation_step_size");
    writer.Double(animation_step_size);

    writeDoubleArray(writer, "double_helix_V_color", double_helix_V_color, 4);

    writer.Key("automatic_camera");
    writer.Bool(automatic_camera);

    writer.Key("preview_editor");
    writer.Bool(preview_editor);

    writer.Key("use_atomic_details");
    writer.Bool(use_atomic_details);

    writer.Key("magic_number");
    writer.Int(magic_number);

    writer.Key("crossover_distance_threshold");
    writer.Double(crossover_distance_threshold);

    writer.Key("crossover_angle_threshold");
    writer.Double(crossover_angle_threshold);

    writer.Key("use_twist");
    writer.Bool(use_twist);

    writer.Key("detect_possible_crossovers");
    writer.Bool(detect_possible_crossovers);

    writer.Key("clear_log_file");
    writer.Bool(clear_log_file);

    writer.Key("test_type");
    writer.Int(test_type);

    writer.Key("display_possible_crossovers");
    writer.Bool(display_possible_crossovers);

    writer.Key("display_nucleotide_basis");
    writer.Bool(display_nucleotide_basis);

    writer.Key("display_base_pairing");
    writer.Bool(display_base_pairing);

    writer.Key("show_overlay");
    writer.Bool(show_overlay);

    writer.Key("usage");
    writer.String(usage.c_str());
    
    writer.EndObject();

    std::ofstream out(DEFAULT_CONFIGPATH);
    out << s.GetString();
    out.close();
  }

  updateConfig();

  configFileWatcher_.addPath(DEFAULT_CONFIGPATH.c_str());

  QObject::connect(&configFileWatcher_, SIGNAL(fileChanged(const QString &)), this, SLOT(updateConfig()));
}

void SEConfig::updateConfig() {
  FILE* fp = fopen(DEFAULT_CONFIGPATH.c_str(), "rb");
  if (fp != NULL) {
    char readBuffer[65536];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    setting_.ParseStream(is);

    show_overlay = setting_["show_overlay"].GetBool();
    min_melting_temp = setting_["min_melting_temp"].GetDouble();
    max_melting_temp = setting_["max_melting_temp"].GetDouble();
    min_gibbs_free_energy = setting_["min_gibbs_free_energy"].GetDouble();
    interpolate_dimensions = setting_["interpolate_dimensions"].GetBool();
    animation_step_size = setting_["animation_step_size"].GetDouble();
    usage = setting_["usage"].GetString();
    use_atomic_details = setting_["use_atomic_details"].GetBool();
    clear_log_file = setting_["clear_log_file"].GetBool();
    test_type = setting_["test_type"].GetInt();
    magic_number = setting_["magic_number"].GetInt();
    automatic_camera = setting_["automatic_camera"].GetBool();
    preview_editor = setting_["preview_editor"].GetBool();
    crossover_distance_threshold = setting_["crossover_distance_threshold"].GetDouble();
    crossover_angle_threshold = setting_["crossover_angle_threshold"].GetDouble();
    display_possible_crossovers = setting_["display_possible_crossovers"].GetBool();
    display_nucleotide_basis = setting_["display_nucleotide_basis"].GetBool();
    use_twist = setting_["use_twist"].GetBool();
    detect_possible_crossovers = setting_["detect_possible_crossovers"].GetBool();
    display_base_pairing = setting_["display_base_pairing"].GetBool();

    Value& double_helix_V_colorVal = setting_["double_helix_V_color"];
    readDoubleArray(double_helix_V_colorVal, double_helix_V_color, 4);
  }
}

void SEConfig::writeDoubleArray(Writer<StringBuffer> & writer, std::string key, double * arr, int length) {
  writer.Key(key.c_str());

  writer.StartArray();
  for (int i = 0; i < length; ++i) {
    writer.Double(arr[i]);
  }
  
  writer.EndArray();
}

void SEConfig::readDoubleArray(Value & val, double * arr, int length) {
  for (int i = 0; i < length; ++i) {
    arr[i] = val[i].GetDouble();
  }
}

