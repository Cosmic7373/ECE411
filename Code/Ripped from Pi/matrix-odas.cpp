#include <json.h>
#include <math.h>
#include <matrix_hal/everloop.h>
#include <matrix_hal/everloop_image.h>
#include <matrix_hal/matrixio_bus.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <array>
#include <iostream>

namespace hal = matrix_hal;

// ENERGY_COUNT : Number of sound energy slots to maintain.
#define ENERGY_COUNT 36
// MAX_VALUE : controls smoothness
#define MAX_VALUE 200
// INCREMENT : controls sensitivity
#define INCREMENT 20
// DECREMENT : controls delay in the dimming
#define DECREMENT 1
// MAX_BRIGHTNESS: Filters out low energy
// Min threshhold was 10 - Bliss
#define MIN_THRESHOLD 50
// MAX_BRIGHTNESS: 0 - 255
// Max was 50 - Bliss
#define MAX_BRIGHTNESS 100

double x, y, z, E;
int energy_array[ENERGY_COUNT];
const double leds_angle_mcreator[35] = {
    170, 159, 149, 139, 129, 118, 108, 98,  87,  77,  67,  57,
    46,  36,  26,  15,  5,   355, 345, 334, 324, 314, 303, 293,
    283, 273, 262, 252, 242, 231, 221, 211, 201, 190, 180};

const double led_angles_mvoice[18] = {170, 150, 130, 110, 90,  70,
                                      50,  30,  10,  350, 330, 310,
                                      290, 270, 250, 230, 210, 190};

//-------------------------------------ECE411--------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

int max_energy = 0;
//-------------------------------------ECE411--------------------------------------------------

void increase_pots() {
  // Convert x,y to angle. TODO: See why x axis from ODAS is inverted
  double angle_xy = fmodf((atan2(y, x) * (180.0 / M_PI)) + 360, 360);
  // Convert angle to index
  int i_angle = angle_xy / 360 * ENERGY_COUNT;  // convert degrees to index
  // Set energy for this angle
  energy_array[i_angle] += INCREMENT * E;
  // Set limit at MAX_VALUE
  energy_array[i_angle] =
      energy_array[i_angle] > MAX_VALUE ? MAX_VALUE : energy_array[i_angle];
}

void decrease_pots() {
  for (int i = 0; i < ENERGY_COUNT; i++) {
    energy_array[i] -= (energy_array[i] > 0) ? DECREMENT : 0;
  }
}

void json_parse_array(json_object *jobj, char *key) {
  // Forward Declaration
  void json_parse(json_object * jobj);
  enum json_type type;
  json_object *jarray = jobj;
  if (key) {
    if (json_object_object_get_ex(jobj, key, &jarray) == false) {
      printf("Error parsing json object\n");
      return;
    }
  }

  int arraylen = json_object_array_length(jarray);
  int i;
  json_object *jvalue;

  for (i = 0; i < arraylen; i++) {
    jvalue = json_object_array_get_idx(jarray, i);
    type = json_object_get_type(jvalue);

    if (type == json_type_array) {
      json_parse_array(jvalue, NULL);
    } else if (type != json_type_object) {
    } else {
      json_parse(jvalue);
    }
  }
}

void json_parse(json_object *jobj) {
  enum json_type type;
  unsigned int count = 0;
  decrease_pots();
  json_object_object_foreach(jobj, key, val) {
    type = json_object_get_type(val);
    switch (type) {
      case json_type_boolean:
        break;
      case json_type_double:
        if (!strcmp(key, "x")) {
          x = json_object_get_double(val);
        } else if (!strcmp(key, "y")) {
          y = json_object_get_double(val);
        } else if (!strcmp(key, "z")) {
          z = json_object_get_double(val);
        } else if (!strcmp(key, "E")) {
          E = json_object_get_double(val);
        }
        increase_pots();
        count++;
        break;
      case json_type_int:
        break;
      case json_type_string:
        break;
      case json_type_object:
        if (json_object_object_get_ex(jobj, key, &jobj) == false) {
          printf("Error parsing json object\n");
          return;
        }
        json_parse(jobj);
        break;
      case json_type_array:
        json_parse_array(jobj, key);
        break;
    }
  }
}

int main(int argc, char *argv[]) {
  // Everloop Initialization
  hal::MatrixIOBus bus;
  if (!bus.Init()) return false;
  hal::EverloopImage image1d(bus.MatrixLeds());
  hal::Everloop everloop;
  everloop.Setup(&bus);

  // Clear all LEDs
  for (hal::LedValue &led : image1d.leds) {
    led.red = 0;
    led.green = 0;
    led.blue = 0;
    led.white = 0;
  }
  everloop.Write(&image1d);

  char verbose = 0x00;

  int server_id;
  struct sockaddr_in server_address;
  int connection_id;
  char *message;
  int messageSize;

  int c;
  unsigned int portNumber = 9001;
  const unsigned int nBytes = 10240;

  server_id = socket(AF_INET, SOCK_STREAM, 0);

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_port = htons(portNumber);

  printf("Binding socket........... ");
  fflush(stdout);
  bind(server_id, (struct sockaddr *)&server_address, sizeof(server_address));
  printf("[OK]\n");

  printf("Listening socket......... ");
  fflush(stdout);
  listen(server_id, 1);
  printf("[OK]\n");

  printf("Waiting for connection in port %d ... ", portNumber);
  fflush(stdout);
  connection_id = accept(server_id, (struct sockaddr *)NULL, NULL);
  printf("[OK]\n");

  message = (char *)malloc(sizeof(char) * nBytes);

  printf("Receiving data........... \n\n");

//-------------------------------------ECE411--------------------------------------------------
  int file_i2c;
  int length;
  unsigned char buffer[60] = {0};
	
  // OPEN THE I2C BUS
  char * filename = (char*)"/dev/i2c-1";

  if ((file_i2c = open(filename, O_RDWR)) < 0) {
 	// ERROR
	printf("Failed to open the i2c bus");
		return 0;
  } 

  // Setting AtMega32u4 as a I2C slave at address 04
  // Confirm with command "i2c -y 1"
  int addr = 0x04;
  if (ioctl(file_i2c, I2C_SLAVE, addr) < 0) {
	printf("Failed to acquire bus access and/or talk to slave.\n");
	// ERROR HANDLING
	return 0;
  }

  // WRITE
  char buff[60] = {'\n'};
  //int x = 370;
//-------------------------------------ECE411--------------------------------------------------

  while ((messageSize = recv(connection_id, message, nBytes, 0)) > 0) {
    message[messageSize] = 0x00;

     //printf("message: %s\n\n", message);
    json_object *jobj = json_tokener_parse(message);
    json_parse(jobj);

    for (int i = 0; i < bus.MatrixLeds(); i++) {
      // led index to angle
      
      int led_angle = bus.MatrixName() == hal::kMatrixCreator
                          ? leds_angle_mcreator[i]
                          : led_angles_mvoice[i];
      // Convert from angle to pots index
      int index_pots = led_angle * ENERGY_COUNT / 360;

      // Mapping from pots values to color
      int color = energy_array[index_pots] * MAX_BRIGHTNESS / MAX_VALUE;
      // Removing colors below the threshold
      color = (color < MIN_THRESHOLD) ? 0 : color;
      
      //-------------------------------------ECE411-------------------------------
      // Bliss Isolated angle relative to "color" intensity
      if (color >= max_energy) {
	      // Threshhold control, so only the loudest sound gets reacted to
	      max_energy = color;
	      printf("Angle: %d  | Color: %d\n", led_angle, color);
	      
	      sprintf(buff, "%d", led_angle); // Copying angle into buffer
	      length = sizeof led_angle - 1;
	      /* // Debugging
	      for (int i = 0; i < 3; i++) {
		      printf("Byte in HEX is %x\n", buff[i]);
	      }
	      printf("Angle in HEX %X DECIMAL Angle %d and length %d\n", led_angle, led_angle, length);
	      */
	      // Sending angle over i2c bus
	      if (write(file_i2c, buff, length) != length) {
		      printf("Failed to write to the i2c bus.\n");
	      }	
      }
    //-------------------------------------ECE411-------------------------------

      image1d.leds[i].red = color * 4;
      image1d.leds[i].green = color;
      image1d.leds[i].blue = 0;
      image1d.leds[i].white = 0;
    }
    everloop.Write(&image1d);

    //-------------------------------------ECE411-------------------------------
    // Bliss
    max_energy = 15;
    //-------------------------------------ECE411-------------------------------
  }
}
