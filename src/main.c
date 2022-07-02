#include <pthread.h>
#include <string.h>

#include "datset_parser/datset_parser.c"
#include "datset_parser/datset_parser.h"
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

  DatasetLabel dataset_label = new_dataset_label("dataset/labels");
  DatasetImage dataset_image = new_dataset_image("dataset/images");
  GRID_SIZE = dataset_image.row_count;
  GRID_SIZE = 28;

  InputNeuronLayer network_input =
      new_input_layer(dataset_image.row_count * dataset_image.col_count);
  HiddenNeuronLayer network_hidden_a =
      new_hidden_layer(network_input.node_count, 16);
  HiddenNeuronLayer network_hidden_b =
      new_hidden_layer(network_hidden_a.node_count, 16);
  OutputNeuronLayer network_output =
      new_output_layer(network_hidden_b.node_count, 10);
  HiddenNeuronLayer network_hidden_array[] = {network_hidden_a,
                                              network_hidden_b};
  NeuralNetwork network = {.input_neuron_layer = network_input,
                           .hidden_neuron_layer_array = network_hidden_array,
                           .hidden_neuron_count = sizeof(network_hidden_array) /
                                                  sizeof(HiddenNeuronLayer),
                           .output_neuron_layer = network_output};
  neural_network_fill_random(&network, -1, 1);
  /* for (int i = 0; i < dataset_image.row_count; i++) { */
  /*   for (int j = 0; j < dataset_image.col_count; j++) { */
  /*     float value = get_image_data(&dataset_image, 0, i, j) / 255.0; */
  /*     set_matrix_element(&network.input_neuron_layer.node_value_matrix,
   * value, */
  /*                        0, i * 28 + j); */
  /*   } */
  /* } */
  /* neural_network_forward_propogation(&network); */
  /* print_matrix(&network.output_neuron_layer.node_value_matrix); */
  /* exit(EXIT_SUCCESS); */

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

  float cells[GRID_SIZE * GRID_SIZE];
  Grid grid = new_grid(cells);
  int index = rand() % 60000;
  KeyState state_prev;

  while (!user_quit) {
    if (mouse.middle_button == KEYDOWN && state_prev == KEYUP) {
      neural_network_fill_random(&network, -1, 1);
    }
    Matrix fk = grid_cells_to_matrix(&grid);
    /* Matrix fk = new_matrix(1, 28 * 28); */
    fk = row_matrix_from(&fk);
    network.input_neuron_layer.node_value_matrix = fk;
    neural_network_forward_propogation(&network);
    print_matrix(&network.output_neuron_layer.node_value_matrix);
    free_matrix(&fk);
    neural_network_print_output(&network);
    SDL_SetRenderDrawColor(RENDERER_PTR, 0, 0, 0, 255);
    SDL_RenderClear(RENDERER_PTR);
    SDL_SetRenderDrawColor(RENDERER_PTR, 255, 255, 255, 255);
    paint_grid(&grid);
    render_grid(&grid);
    render_brush();
    state_prev = mouse.middle_button;
    SDL_RenderPresent(RENDERER_PTR);
  }
  exit(EXIT_SUCCESS);
}
