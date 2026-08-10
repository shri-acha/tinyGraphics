#include "src/_graphics.h"
#include "graphics.h"
#include "types.h"
#define MAX_OBJECTS_IN_A_SCENE 128

int loadRenderedObjectToContext(renderContext* rc, renderedObject* ro){
	if ( 	 rc == NULL ||
			 rc->scene_context == NULL ) {
		return -1;
	}
	if (rc->scene_context->objs != NULL) {
		rc->scene_context->objs[rc->scene_context->no_of_objs++] = *ro;
	}else {
		rc->scene_context = newSceneContext();
		rc->scene_context->no_of_objs = 0;
		rc->scene_context->objs = malloc(sizeof(renderedObject) * MAX_OBJECTS_IN_A_SCENE);
	}
	return 0;
}
