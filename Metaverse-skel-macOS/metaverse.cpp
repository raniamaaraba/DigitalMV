#include "metaverse.h"
#include "display.h"
#include <cctype>
#include <fstream>
#include <ios>
#include <iostream>
#include <istream>
#include <unistd.h>
#include <vector>


//4
/*
 * count neighbours
 *
 * based on the given locations, it determines the occupied neighbours
 *
 * input: board, row, column
 * output: the number of occupied neighbours
 */
int count_neighbors(const metaverse_t &board, int row, int column) {
  int counter = 0;
  int c = -1;

  for (;c < 2; c++) {
    int r = -1;
    for (;r < 2; r++){
      if (row + r >= 0 && row + r < board.size() && column + c >= 0 && column + c < board.size()) {
        if (r==0 & c ==0){
          continue;
        }
        if (board[row + r][column + c] == 1){
          counter++;
        }
      }
    }
  }
  return counter;
}


//6
/*
 * occupied in next tick
 *
 * determines wheter or not the neighbours are still occupied by the next rendition
 *
 * input: currently occupied, neighbour count
 * output: occupied neighbours
 *         
 */
bool occupied_in_next_tick(bool currently_occupied, int neighbor_count) {
  if(currently_occupied == true  && (neighbor_count == 2 || neighbor_count == 3)){
    return true;
  } else if (currently_occupied == false && neighbor_count == 3){
    return true;
  }
  return false;
}
//5
/*
 * tick
 *
 * creates the next generation metaverse
 *
 * input: board
 * output: returns a new metaverse
 */
metaverse_t tick(const metaverse_t &board) {
  metaverse_t next;
  resize_metaverse(board.size(),next);
  for (int c = 0; c < board.size(); c++){
    for (int r = 0; r < board.size(); r++){
      int neighbours = count_neighbors(board, c, r);
      next[c][r] = occupied_in_next_tick(board[c][r], neighbours);
    }
  }
  return next;
}

//2
/*
 * resize metaverse
 *
 * resizes the metaverse based on the read values
 *
 * input: rows, board, metaverse t
 * output: the board and returns a true value
 */
bool resize_metaverse(int rows, metaverse_t &board) {
    board.resize(rows, std::vector<bool>(rows));
  return true;
}

//3
/*
 * citizenship row to metaverse row
 *
 * ensures that the initial occupancy is the same as the current
 *
 * input: input row, row, board
 * output: resizes the metaverse base on the input row
 */
bool citizenship_row_to_metaverse_row(const std::string &input_row, int row,
                                      metaverse_t &board) {
  //inital occupancy needs to match the size
  if (!(input_row.size() == row)) {
    int count = 0;
    for(count = 0; count < input_row.length(); count++){
      if(input_row[count] == '1'){
        board[row][count] = true;
      }
    }
    return true;
  }
  return false;
}


//1
/*
 * read metaverse configuration line from file
 *
 * determines whether or not a size and generation can be read from a file
 *
 * input: metaverse_file (file being read), size (generation size), generations
 * output: returns a true or false value if the generations and size can be correctly read or not
 */
bool read_metaverse_configuration_line_from_file(std::ifstream &metaverse_file,
                                                 int &size, int &generations) {
  if (!(metaverse_file>>size)){
    return false;
  }
  char comma;
  if(!(metaverse_file>> comma)){
    return false;
  }
  if (!(metaverse_file>>generations)){
    return false;
  }
  return true;
}
bool initialize_metaverse_from_file(std::ifstream &metaverse_file,
                                    metaverse_t &metaverse,
                                    int &generations) {
  int size{};
  int actual_row{};

  generations = 0;

  if (!read_metaverse_configuration_line_from_file(metaverse_file, size,
                                                   generations)) {
    return false;
  }

  if (!resize_metaverse(size, metaverse)) {
    return false;
  }

  metaverse_file >> std::ws;
  while (!metaverse_file.eof()) {
    std::string line{};
    if (!std::getline(metaverse_file, line)) {
      return false;
    }
    if (!citizenship_row_to_metaverse_row(line, actual_row, metaverse)) {
      return false;
    }
    actual_row++;
    metaverse_file >> std::ws;
  }
  return actual_row == size;
}

//7
/*
 * model metaverse
 *
 * prints the state of the metaverse between generations
 *
 * input: starting metaverse, generations
 * output: state of metaverse
 */
void model_metaverse(const metaverse_t &starting_metaverse, int generations) {
  int gen = 0;
  metaverse_t new_metaverse{starting_metaverse};
  for (gen = 0; gen < generations; gen++){
    display_metaverse(std::cout,new_metaverse);
    new_metaverse = tick(new_metaverse);
  }
  return;
}

