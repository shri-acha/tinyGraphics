#include "src/_graphics.h"
#include "graphics.h"
#include "types.h"
#define MAX_OBJECTS_IN_A_SCENE 128

/* Copies *ro into the scene context and takes ownership of (frees) ro. */
int loadRenderedObjectToContext(renderContext* rc, renderedObject* ro){
	if (ro == NULL) return -1;
	if (rc == NULL || rc->scene_context == NULL) {
		free(ro);
		return -1;
	}
	sceneContext* sc = rc->scene_context;
	if (sc->objs == NULL) {
		sc->no_of_objs = 0;
		sc->objs = malloc(sizeof(renderedObject) * MAX_OBJECTS_IN_A_SCENE);
		if (sc->objs == NULL) {
			free(ro);
			return -1;
		}
	}
	if (sc->no_of_objs >= MAX_OBJECTS_IN_A_SCENE) {
		free(ro);
		return -1;
	}
	sc->objs[sc->no_of_objs++] = *ro;
	free(ro);
	return 0;
}
