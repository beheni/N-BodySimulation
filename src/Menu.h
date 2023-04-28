#include <vector>
#include <string>
#include <functional>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

enum class WidgetType {
    Button,
    SliderFloat,
    ColorEdit3
};

class Menu {
public:
    Menu(const std::string& title, GLFWwindow* window, int flags = 0);
    ~Menu();

    void AddButton(const std::string& label, const std::function<void()>& callback) 
    {
        m_Widgets.emplace_back([label, callback] {
            if (ImGui::Button(label.c_str())) 
            {
                callback();
            }
            });
    }
    void AddSliderFloat(const std::string& label, float* value, float minValue, float maxValue) 
    {
        m_Widgets.emplace_back([label, value, minValue, maxValue] {
            ImGui::SliderFloat(LabelPrefix(label).c_str(), value, minValue, maxValue);
            });
    }
    void AddCheckbox(const std::string& label, bool* value) 
    {
        m_Widgets.emplace_back([label, value] {
            ImGui::Checkbox(label.c_str(), value);
            });
    }
    void AddText(const std::string& label)
    {
        m_Widgets.emplace_back([label] {
            ImGui::Text(label.c_str());
            });
    }

    void Render();

    // Imgui styles from @janekb04 
    // https://github.com/ocornut/imgui/issues/707#issuecomment-917151020
    static void EmbraceTheDarkness();

private:
    void Update();

    static std::string LabelPrefix(const std::string& label);

private:
    int m_Flags;
    std::string m_Title;
    std::vector<std::function<void()>> m_Widgets;
};
