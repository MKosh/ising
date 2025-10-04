#include "window.hpp"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include <print>

const i32 size = 64;
const i32 size2 = size*size;
double values[size*size];

inline double RandomRange(double min, double max) {
    double scale = rand() / (double) RAND_MAX;
    return min + scale * ( max - min );
}

int main() {
  GLFWwindow* window;
  initWindow(window, 1280, 720, "Ising Model");
  std::print("Window ready!\n");
  float thresh = 0.5;
  int steps = 1;

  for (int i = 0; i < size2; ++i) {
    values[i] = 1.0;
  }
  double temp = 0.;
  int x = 0;
  int y = 0;
  srand(0);
  bool running = false;

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    if (ImGui::BeginMainMenuBar()) {
      ImGui::EndMainMenuBar();
    }

    if (ImGui::Begin("Controls")) {
      ImGui::SliderFloat("Thresh", &thresh, 0.0f, 1.0f);
      ImGui::SliderInt("Steps", &steps, 1, 20);
      if (ImGui::Button("Play/Pause", ImVec2(100, 20))) {
        running = !running;
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset", ImVec2(100, 20))) {
        for (int i = 0; i < size2; ++i) {
          values[i] = 1.0;
        }
      }
      ImGui::End();
    }

    if (running) {
      for (int i = 0; i < steps; ++i) {
        temp = RandomRange(0., 1.);
        x = RandomRange(0, size);
        y = RandomRange(0, size);
        if (temp >= thresh) {
          values[x*size + y] = -1*values[x * size + y];
        }
      }
    }

    if (ImGui::Begin("Simulation")) {
      if (ImPlot::BeginPlot("2D Ising Model", ImVec2(-1,-1))) {
        ImPlot::PushColormap(ImPlotColormap_Greys);
        ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoDecorations, ImPlotAxisFlags_NoDecorations);
        ImPlot::PlotHeatmap("", values, size, size, 0, 1, nullptr, ImPlotPoint(0,0), ImPlotPoint(1,1), ImPlotHeatmapFlags_None);
        ImPlot::EndPlot();
        ImPlot::PopColormap();
      }
      ImGui::End();
    }

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  std::print("Done!\n");
}
