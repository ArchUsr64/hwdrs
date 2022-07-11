#include <SDL2/SDL_timer.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "datset_parser/datset_parser.c"
#include "global_vars.c"
#include "gui/brush.c"
#include "gui/grid.c"
#include "gui/gui.h"
#include "gui/paint.c"
#include "input/input.c"
#include "logger.c"
#include "math/math.h"
#include "math/matrix.c"
#include "math/util.c"
#include "math/util.h"
#include "math/vector.c"
#include "neural_network/neural_network.c"
#include "neural_network/neuron_layer.c"
#include "window/map.c"

int main() {

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    log_("Failed to initialise SDL: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  pthread_t input_thread_id;
  pthread_create(&input_thread_id, NULL, input_thread, NULL);
  WINDOW_PTR = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, INITIAL_WINDOW_WIDTH,
                                INITIAL_WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!WINDOW_PTR) {
    log_("Failed to create window: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }
  RENDERER_PTR = SDL_CreateRenderer(
      WINDOW_PTR, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (!RENDERER_PTR) {
    log_("Failed to create renderer: %s", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  DatasetLabel training_label = new_dataset_label("dataset/training/labels");
  DatasetImage training_image = new_dataset_image("dataset/training/images");
  DatasetLabel test_label = new_dataset_label("dataset/test/labels");
  DatasetImage test_image = new_dataset_image("dataset/test/images");
  GRID_SIZE = training_image.row_count;

  InputNeuronLayer network_input =
      new_input_layer(training_image.row_count * training_image.col_count);
  /* HiddenNeuronLayer network_hidden = */
  /*     new_hidden_layer(network_input.node_count, 8); */
  OutputNeuronLayer network_output =
      new_output_layer(network_input.node_count, 10);
  HiddenNeuronLayer network_hidden_array[] = {
      /* network_hidden, */
  };
  NeuralNetwork network = {.input_neuron_layer = network_input,
                           .hidden_neuron_layer_array = network_hidden_array,
                           .hidden_neuron_count = sizeof(network_hidden_array) /
                                                  sizeof(HiddenNeuronLayer),
                           .output_neuron_layer = network_output,
                           .nodes_updated = false};
  neural_network_fill_random(&network, 0, 0.5);

  LEARNING_RATE = 0.01;
  for (int i = 0; i < training_image.image_count; i++) {
    Matrix image_data = get_image_data_as_matrix(&training_image, i);
    matrix_scale(&image_data, 1.0 / 255);
    int digit = get_label_data(&training_label, i);
    Matrix desired_output_matrix = desired_output_if_number_is(digit);
    to_row_matrix(&image_data);
    neural_network_set_input_matrix(&network, &image_data);
    neural_network_forward_propagation(&network);
    neural_network_backward_propogation(&network, &desired_output_matrix,
                                        LEARNING_RATE);
    if (i % 1000 == 0) {
      float fitness =
          neural_network_evaluate_fitness(&network, &test_image, &test_label);
      log_("Current fitness: %5f @ %d%% training", fitness,
           (int)((100.0 * i) / training_image.image_count));
    }
    free_matrix(&desired_output_matrix);
    free_matrix(&image_data);
  }
  neural_network_print_output(&network);

  float cells[GRID_SIZE * GRID_SIZE];
  Grid grid = new_grid(cells);
  while (!user_quit) {
    {
      SDL_SetRenderDrawColor(RENDERER_PTR, 0, 0, 0, 255);
      SDL_RenderClear(RENDERER_PTR);
      SDL_SetRenderDrawColor(RENDERER_PTR, 255, 255, 255, 255);
    }
    if (mouse.middle_button == KEYDOWN) {
      clear_grid(&grid);
    }
    set_cursor_origin();
    Matrix nn_input_matrix = grid_cells_to_matrix(&grid);
    to_row_matrix(&nn_input_matrix);
    neural_network_set_input_matrix(&network, &nn_input_matrix);
    neural_network_forward_propagation(&network);
    free_matrix(&nn_input_matrix);
    neural_network_print_output(&network);
    paint_grid(&grid);
    render_grid(&grid);
    render_brush();
    { SDL_RenderPresent(RENDERER_PTR); }
  }
  exit(EXIT_SUCCESS);
}
