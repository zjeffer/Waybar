#pragma once

#include <gtkmm/button.h>
#include <gtkmm/label.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "AModule.hpp"
#include "bar.hpp"
#include "modules/hyprland/backend.hpp"

namespace waybar::modules::hyprland {

class Workspaces;

enum class SORT_METHOD { ID = 0, NAME, NUMBER, DEFAULT };

class Workspace {
 public:
  explicit Workspace(const Json::Value& workspace_data, Workspaces& workspace_manager);
  std::string& select_icon(std::map<std::string, std::string>& icons_map);
  Gtk::Button& button() { return button_; };

  int id() const { return id_; };
  std::string name() const { return name_; };
  std::string output() const { return output_; };
  bool active() const { return active_; };
  bool is_special() const { return is_special_; };
  bool is_persistent() const { return is_persistent_; };
  bool is_visible() const { return is_visible_; };
  bool is_empty() const { return windows_ == 0; };
  bool is_urgent() const { return is_urgent_; };

  auto handle_clicked(GdkEventButton* bt) -> bool;
  void set_active(bool value = true) { active_ = value; };
  void set_persistent(bool value = true) { is_persistent_ = value; };
  void set_urgent(bool value = true) { is_urgent_ = value; };
  void set_visible(bool value = true) { is_visible_ = value; };
  void set_windows(uint value) { windows_ = value; };
  void set_name(std::string value) { name_ = value; };

  void update(const std::string& format, const std::string& icon);

 private:
  Workspaces& workspace_manager_;

  int id_;
  std::string name_;
  std::string output_;
  uint windows_;
  bool active_ = false;
  bool is_special_ = false;
  bool is_persistent_ = false;
  bool is_urgent_ = false;
  bool is_visible_ = false;

  Gtk::Button button_;
  Gtk::Box content_;
  Gtk::Label label_;
};

class Workspaces : public AModule, public EventHandler {
 public:
  Workspaces(const std::string&, const waybar::Bar&, const Json::Value&);
  ~Workspaces() override;
  void update() override;
  void init();

  auto all_outputs() const -> bool { return all_outputs_; }
  auto show_special() const -> bool { return show_special_; }
  auto active_only() const -> bool { return active_only_; }

  auto get_bar_output() const -> std::string { return bar_.output->name; }

 private:
  void onEvent(const std::string&) override;
  void update_window_count();
  void sort_workspaces();
  void create_workspace(Json::Value& value);
  void remove_workspace(std::string name);
  void set_urgent_workspace(std::string windowaddress);
  void parse_config(const Json::Value& config);
  void register_ipc();

  bool all_outputs_ = false;
  bool show_special_ = false;
  bool active_only_ = false;
  SORT_METHOD sort_by_ = SORT_METHOD::DEFAULT;

  void fill_persistent_workspaces();
  void create_persistent_workspaces();
  std::vector<std::string> persistent_workspaces_to_create_;
  bool persistent_created_ = false;

  std::string format_;
  std::map<std::string, std::string> icons_map_;
  bool with_icon_;
  uint64_t monitor_id_;
  std::string active_workspace_name_;
  std::vector<std::unique_ptr<Workspace>> workspaces_;
  std::vector<Json::Value> workspaces_to_create_;
  std::vector<std::string> workspaces_to_remove_;
  std::mutex mutex_;
  const Bar& bar_;
  Gtk::Box box_;
};

}  // namespace waybar::modules::hyprland
