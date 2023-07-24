#include <gtkmm/button.h>
#include <gtkmm/label.h>

#include <memory>

#include "AModule.hpp"
#include "bar.hpp"
#include "modules/hyprland/backend.hpp"

namespace waybar::modules::hyprland {

class Workspace {
 public:
  Workspace(const Json::Value& workspace_data);
  std::string& select_icon(std::map<std::string, std::string>& icons_map);
  Gtk::Button& button() { return button_; };

  int id() const { return id_; };
  std::string name() const { return name_; };
  std::string output() const { return output_; };
  int active() const { return active_; };
  bool is_special() const { return is_special_; };

  auto handle_clicked(GdkEventButton* bt) -> bool;
  void set_active(bool value = true) { active_ = value; };

  void update(const std::string& format, const std::string& icon);

 private:
  int id_;
  std::string name_;
  std::string output_;
  int windows_;
  bool active_;
  bool is_special_;

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

  auto get_bar_output() const -> std::string { return bar_.output->name; }

 private:
  void onEvent(const std::string&) override;
  void sort_workspaces();
  void create_workspace(Json::Value& value);
  void remove_workspace(std::string name);

  bool all_outputs_ = false;
  bool show_special_ = false;

  std::string format_;
  std::map<std::string, std::string> icons_map_;
  bool with_icon_;
  std::string active_workspace_name;
  std::vector<std::unique_ptr<Workspace>> workspaces_;
  std::vector<Json::Value> workspaces_to_create_;
  std::vector<std::string> workspaces_to_remove_;
  std::mutex mutex_;
  const Bar& bar_;
  Gtk::Box box_;
};

}  // namespace waybar::modules::hyprland
