#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#include "http.h"

#define MY_UUID { 0x91, 0x41, 0xB6, 0x28, 0xBC, 0x89, 0x49, 0x8E, 0xB1, 0x47, 0x04, 0x9F, 0x49, 0xC0, 0x99, 0xAD }
PBL_APP_INFO(MY_UUID, "Diary", "Jorge Izquierdo", 1, 0, DEFAULT_MENU_ICON, APP_INFO_STANDARD_APP);
#define APP_COOKIE 1949327671

#define GET_LIST_COOKIE 183872718
#define CHECKIN_COOKIE 190411904
#define MAX_PLACES 3

static int lat, lon;
static bool located = false;
static char str[MAX_PLACES*10][20];
static int mores = -1;
static bool more = true;
static int count = 0;

Window window;
TextLayer logLayer;

SimpleMenuLayer list;
SimpleMenuItem listItems[MAX_PLACES*10];
SimpleMenuSection listSections[1];

//Method definition
static void simpleMenuItemSelectedCallback(int index, void *context);
static void createMenu(int numElements);
static void addMenuItem(int index, char *title);
static void getList();
static void logMessage(char *log);
static void setUpMenu();
static void getMore();

static void setUpMenu(){
	
	//int max = (mores+1)*MAX_PLACES;
	for (int i = 0; i<count; i++){
		
		addMenuItem(i, str[i]);
	}
	if (more && mores<10) {
		
		addMenuItem(count, "More...");
		createMenu(count+1);
	}
	else {
		createMenu(count);
	}
}

static void logMessage(char *log)
{
    text_layer_set_text(&logLayer, log);
    layer_set_hidden(&logLayer.layer, false);

    vibes_double_pulse();
}

void location(float latitude, float longitude, float altitude, float accuracy, void* context) {
	
	//logMessage("'located'");
	lat = latitude * 10000000;
	lon = longitude * 10000000;
	
	located = true;
	getList();
}

void success(int32_t cookie, int http_status, DictionaryIterator* received, void* context) {
	
	//logMessage("sending list...");
	if (cookie == GET_LIST_COOKIE){
			
			mores += 1;
			for (int i=mores*MAX_PLACES; i<(mores+1)*MAX_PLACES; i++) {
			Tuple *value = dict_find(received, i);
			if (value) {
				
				count += 1;
				strcpy(str[i], value->value->cstring);	
			}
				else {
					more = false;
				}
			
       }
		setUpMenu();

    	
	}
	
	else if (cookie == CHECKIN_COOKIE){
		
		logMessage("Checked in");
	}
	
	return;
}
void reconnect(void* context) {
	located = false;
	getList();
}

static void addMenuItem(int index, char *title)
{
    listItems[index] = (SimpleMenuItem){.title = title, .subtitle = NULL, .icon = NULL, .callback = simpleMenuItemSelectedCallback};
}

static void createMenu(int numElements)
{
	layer_remove_from_parent(simple_menu_layer_get_layer(&list));
    // Menu
    listSections[0] = (SimpleMenuSection){.items = listItems, .num_items = numElements, .title = "Places"};

    simple_menu_layer_init(&list,
                           window.layer.bounds,
                           &window,
                           listSections,
                           1,
                           NULL);

    layer_add_child(&window.layer, simple_menu_layer_get_layer(&list));
}

static void simpleMenuItemSelectedCallback(int index, void *context)
{
	if (index == count) {
		getMore();
		return;
	}
	
	DictionaryIterator *body;
	#error Type your own URL
	HTTPResult result = http_out_get("[YOUR URL]/checkin", CHECKIN_COOKIE, &body);
	if(result != HTTP_OK) {
		
		logMessage("http error");
		return;
	}
	dict_write_int32(body, 0, index);
	
	if(http_out_send() != HTTP_OK) {
		
		logMessage("'http error'");
		return;
	}
}
void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Diary");
  window_stack_push(&window, true /* Animated */);
  http_register_callbacks((HTTPCallbacks){.success=success,.reconnect=reconnect,.location=location}, (void*)ctx);
	getList();

	//Log layer	
	text_layer_init(&logLayer, window.layer.bounds);
    text_layer_set_text_color(&logLayer, GColorBlack);
    text_layer_set_background_color(&logLayer, GColorWhite);
    text_layer_set_font(&logLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(&logLayer, GTextAlignmentCenter);
    text_layer_set_text(&logLayer, "text");
    layer_add_child(&window.layer, &logLayer.layer);
    layer_set_hidden(&logLayer.layer, true);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
	  .messaging_info = {
			.buffer_sizes = {
				.inbound = 124,
				.outbound = 256,
			}
		}
  };
  app_event_loop(params, &handlers);
}

static void getList(){
	
	if (!located){
		
				
		http_location_request();
		return;
	}
	
	DictionaryIterator *body;
	#error Type your own URL
	HTTPResult result = http_out_get("[YOUR URL]/list", GET_LIST_COOKIE, &body);
	if(result != HTTP_OK) {
		
		logMessage("http error");
		return;
	}
	dict_write_int32(body, 0, lat);
	dict_write_int32(body, 1, lon);
	
	if(http_out_send() != HTTP_OK) {
		
		logMessage("'http error'");
		return;
	}
	
}

static void getMore(){
	
	DictionaryIterator *body;
	#error Type your own URL
	HTTPResult result = http_out_get("[YOUR URL]/more", GET_LIST_COOKIE, &body);
	if(result != HTTP_OK) {
		
		logMessage("http error");
		return;
	}
	dict_write_int32(body, 0, lat);
	dict_write_int32(body, 1, lon);
	
	if(http_out_send() != HTTP_OK) {
		
		logMessage("'http error'");
		return;
	}
}
