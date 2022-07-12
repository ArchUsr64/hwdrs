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
#include "sdl/sdl_abstracted.c"
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
  /* HiddenNeuronLayer network_hidden_1 = */
  /*     new_hidden_layer(network_input.node_count, 9); */
  /* HiddenNeuronLayer network_hidden_2 = */
  /*     new_hidden_layer(network_input.node_count, 10); */
  OutputNeuronLayer network_output =
      new_output_layer(network_input.node_count, 10);
  HiddenNeuronLayer network_hidden_array[] = {
      /* network_hidden_1, */
      /* network_hidden_2, */
  };
  NeuralNetwork network = {.input_neuron_layer = network_input,
                           .hidden_neuron_layer_array = network_hidden_array,
                           .hidden_neuron_count = sizeof(network_hidden_array) /
                                                  sizeof(HiddenNeuronLayer),
                           .output_neuron_layer = network_output,
                           .nodes_updated = false};
  neural_network_fill_random(&network, -0.1, 0.1);

  LEARNING_RATE = 0.001;
  for (int i = 0; i < training_image.image_count && true; i++) {
    if (i % 6000 == 0) {
      float fitness =
          neural_network_evaluate_fitness(&network, &test_image, &test_label);
      log_("Current fitness: %.4f @ %d%% training", fitness,
           (int)((100.0 * i) / training_image.image_count));
    }
    int index = i % training_image.image_count;
    Matrix image_data = get_image_data_as_matrix(&training_image, index);
    matrix_scale(&image_data, 1.0 / 255);
    int digit = get_label_data(&training_label, index);
    Matrix desired_output_matrix = desired_output_if_number_is(digit);
    to_row_matrix(&image_data);
    neural_network_set_input_matrix(&network, &image_data);
    neural_network_forward_propagation(&network);
    neural_network_backward_propogation(&network, &desired_output_matrix,
                                        LEARNING_RATE);
    free_matrix(&desired_output_matrix);
    free_matrix(&image_data);
  }
  neural_network_print_output(&network);

  float cells[GRID_SIZE * GRID_SIZE];
  Grid grid = new_grid(cells);
  KeyState previous_state = KEYUP;
  int r = 0;
  while (!user_quit) {
    set_cursor_origin();
    log_("Examining weights for %d", r);
    {
      set_draw_colour(0, 0, 0);
      clear_window();
      set_draw_colour(1, 1, 1);
    }
    if (mouse.middle_button == KEYDOWN && previous_state == KEYUP) {
      r++;
      r %= 10;
      Matrix weight_matrix = new_matrix(28, 28);
      for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
          float cell_value = get_matrix_element(
              &network.output_neuron_layer.node_weight_matrix, i * 28 + j, r);
          set_matrix_element(&weight_matrix, cell_value, i, j);
        }
      }
      matrix_normal_map(&weight_matrix);
      for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
          float cell_value = get_matrix_element(&weight_matrix, i, j);
          cell_value = cell_value * (cell_value > 0);
          set_cell_value(&grid, cell_value, j, i);
        }
      }
      free_matrix(&weight_matrix);
    }
    Matrix nn_input_matrix = grid_cells_to_matrix(&grid);
    to_row_matrix(&nn_input_matrix);
    neural_network_set_input_matrix(&network, &nn_input_matrix);
    neural_network_forward_propagation(&network);
    free_matrix(&nn_input_matrix);
    neural_network_print_output(&network);
    print_matrix(&network.output_neuron_layer.node_bias_matrix);
    paint_grid(&grid);
    render_grid(&grid);
    render_brush();
    if (mouse.left_button == KEYDOWN && mouse.right_button == KEYDOWN) {
      neural_network_render_structure(&network);
    }
    previous_state = mouse.middle_button;
    { present_to_window(); }
  }
  exit(EXIT_SUCCESS);
}
