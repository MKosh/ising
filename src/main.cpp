#include "window.hpp"
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "main.h"
#include <print>
#include <random>

const i32 size = 8;
const i32 size2 = size*size;

std::mt19937 generator{std::random_device{}()};
std::uniform_int_distribution<i32> rand_x(0, size - 1);
std::uniform_int_distribution<i32> rand_y(0, size - 1);

i32 values[size*size];
i32 mcs = 0;
i32 systemEnergy = 0;
i32 magnetization = 0;
i32 demonEnergy = 0;
i32 demonEnergyAccumulator = 0;
i32 systemEnergyAccumulator = 0;
i32 mAccumulator = 0;
i32 m2Accumulator = 0;
i64 acceptedMoves = 0;
f64 norm = 0;
f64 norm2 =0;

struct System {
  i32 systemEnergy;
  i32 magnetization;
};

inline double RandomRange(double min, double max) {
    double scale = rand() / (double) RAND_MAX;
    return min + scale * ( max - min );
}

void resetGrid(i32* values, i32 size) {
  initGrid(values, size);
  mcs = 0;
  magnetization = 0;
  demonEnergy = 0;
  systemEnergy = 0;
  mAccumulator = 0;
  m2Accumulator = 0;
  systemEnergyAccumulator = 0;
  demonEnergyAccumulator = 0;
  acceptedMoves = 0;
}

i32 getDeltaEnergy(i32& x, i32& y) {
  x = rand_x(generator);
  y = rand_y(generator);
  i32 left_x = (x - 1 + size)%size;
  i32 left_y = y;
  i32 right_x = (x + 1)%size;
  i32 right_y = y;
  i32 up_x = x;
  i32 up_y = (y + 1)%size;
  i32 down_x = x;
  i32 down_y = (y-1+size)%size;
  i32 left = values[left_x * size + left_y];
  i32 right = values[right_x * size + right_y];
  i32 up = values[up_x * size + up_y];
  i32 down = values[down_x * size + down_y];
  i32 dE = 2 * values[x * size + y] * (left + right + up + down); 
#ifdef DEBUG
  if (dE > 10 or dE < -10) {
    std::print("(x, y) = ({}, {})\n", x, y);
    std::print("left : ({}, {})\n", left_x, left_y);
    std::print("right: ({}, {})\n", right_x, right_y);
    std::print("up   : ({}, {})\n", up_x, up_y);
    std::print("down : ({}, {})\n", down_x, down_y);
  }
#endif
  return dE;
}

void initGrid(i32* values, i32 size) {
  i32 size2 = size*size;
  for (i32 i = 0; i < size2; ++i) {
    values[i] = 1;
  }
  i32 tries = 0;
  i32 E = -size2;
  i32 dE = 0;
  i32 x = -1;
  i32 y = -1;
  while (E<systemEnergy && tries < 10*size2) {
    dE = getDeltaEnergy(x, y);
    if (dE > 0) {
      E += dE;
      values[x * size + y] *= -1;
      magnetization += 2 * values[x * size + y];
    }
    tries++;
  }
  systemEnergy = E;
}

void doMcStep(i32* values, i32 size) {
  i32 size2 = size * size;
  i32 dE = 0;
  i32 x = -1;
  i32 y = -1;
  for (i32 i = 0; i < size2; ++i) {
    dE = getDeltaEnergy(x, y);
#ifdef DEBUG
    std::print("dE: {}\n", dE);
#endif
    if (dE <= demonEnergy) {
      i32 newSpin = -1 * values[x * size + y];
      values[x * size + y] = newSpin;
      acceptedMoves++;
      systemEnergy += dE;
      demonEnergy -= dE;
      magnetization += 2 * newSpin;
    }
    systemEnergyAccumulator += systemEnergy;
    demonEnergyAccumulator += demonEnergy;
    mAccumulator += magnetization;
    m2Accumulator += magnetization * magnetization;
  }
  mcs++;
}

int main() {
  GLFWwindow* window;
  initWindow(window, 1280, 720, "Ising Model");
  std::print("Window ready!\n");

  bool running = false;
  bool step = false;
  initGrid(values, size);

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

    norm = 1.0/(mcs * size2);
    norm2 = 1.0/mcs;
    if (ImGui::Begin("Controls")) {
      ImGui::Text("%d MC steps", mcs);
      ImGui::Text("%g Norm", norm);
      ImGui::Text("%g Norm 2", norm2);
      ImGui::Text("%g Accpet", acceptedMoves*norm2);
      ImGui::Text("%g systemEnergyAcc", systemEnergyAccumulator*norm2);
      ImGui::Text("%g demonEnergyAcc", demonEnergyAccumulator*norm);
      ImGui::Text("%g M", norm*mAccumulator);
      ImGui::Text("%g M^2", norm*m2Accumulator);
      ImGui::Text("%g M2", norm2*mAccumulator);
      ImGui::Text("%g M2^2", norm2*m2Accumulator);
      if (ImGui::Button("Start/Stop", ImVec2(100, 20))) {
        running = !running;
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset", ImVec2(100, 20))) {
        resetGrid(values, size);
      }
      if(ImGui::Button("Step", ImVec2(-1,20))) {
        step = true;
      }
      ImGui::End();
    }

    if (running and !step) {
      doMcStep(values, size);
    }
    if (step and !running) {
      doMcStep(values, size);
      step = false;
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
