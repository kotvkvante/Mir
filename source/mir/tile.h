#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#define PATTERN_TILE_INFO_STR(t) "Tile[%d, %d] info:\n\tField: %s\n\tLandscape: %s\n\tBuilding: %s\n\tUnit: %s", \
                                 t->x, t->y, \
                                 str_field_info[(int)t->entities[FIELD]], \
                                 str_landscape_info[(int)t->entities[LANDSCAPE]], \
                                 str_building_info[(int)t->entities[BUILDING]], \
                                 str_unit_info[(int)t->entities[UNIT]]

#define PATTERN_TILE_INFO_WSTR(t) L"Tile[%d, %d] info:\n\tField: %ls\n\tLandscape: %ls\n\tBuilding: %ls\n\tUnit: %ls %dnd devision\n\t [%d %d %d(e) %d(t) %d(h)]", \
                                  t->x, t->y, \
                                  wstr_field_info[(int)t->entities[FIELD]], \
                                  wstr_landscape_info[(int)t->entities[LANDSCAPE]], \
                                  wstr_building_info[(int)t->entities[BUILDING]], \
                                  wstr_unit_info[(int)t->entities[UNIT]], dev, x, y, e, team, health


enum tile_entity_types {FIELD, LANDSCAPE, BUILDING, UNIT};

enum field_types     { NO_FIELD     = 0, PLAINS  = 1, SEA, FIELDS_COUNT};
enum lanscape_types  { NO_LANDSCAPE = 0, FOREST  = 1, MOUNTAIN, MOUNTAIN_FOREST, LANDSCAPE_COUNT};
enum building_types  { NO_BUILDING  = 0, TOWER   = 1, VILLAGE, BUILDING_COUNT };
enum unit_types      { NO_UNIT      = 0, WARRIOR = 1, ARCHER, SHIP, CANNON,UNIT_COUNT };


typedef struct tile_t tile_t;
typedef struct unit_t unit_t;

typedef struct tile_t
{
    int x, y;
    char color;
    char entities[4];

    unit_t* unit;


    // depr
    int field_id; // field texture
    int field_type;
    int building_id;
    int landscape_id;
    int unit_id;
} tile_t;

void tiles_init();

void tile_rand_entities(tile_t* tile);
void tile_rand_field(tile_t* tile);
void tile_rand_entity_or_nothing(tile_t* tile, int entity);


void tile_set_entity(tile_t* tile, char entity_type, char entity_code);
void tile_reset_entities(tile_t* t);

char tile_get_entity(tile_t* tile, char entity_type);
void tile_move_entity(tile_t* tile_from, char entity_type, tile_t* tile_dest);

void tile_get_info_wstr(tile_t* tile, wchar_t* str, int size);



void tile_draw_begin();
void tile_draw_prepare(int x, int y);
void tile_draw_end();

void _tile_draw(int texture);
void tile_draw(tile_t* tile);
void tile_draw_active();
void tile_draw_info(tile_t* tile);


//void tile_draw_selected(void);
void tile_draw_selected(int x, int y);
void tile_draw_selected_l(int x, int y);
void tile_draw_hovered_l(int x, int y);

void tile_print_info(tile_t* t);

void tiles_test_draw();


#endif // TILE_H_INCLUDED
