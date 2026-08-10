#include "graphics.h"
#include "types.h"
#include "src/utils.h"
#include <limits.h>

frameBuffer *createFrameBuffer(int width, int height) {
  pixelBuffer *buffer = calloc(width * height, sizeof(pixelBuffer));
  int *depth_buffer = malloc(width * height * sizeof(int));
  for (int i = 0; i < width * height; i++) {
    depth_buffer[i] = INT_MAX;
  }
  frameBuffer *fb = (frameBuffer *)malloc(sizeof(frameBuffer));
  fb->height = height;
  fb->width = width;
  fb->buffer = buffer;
  fb->depth_buffer = depth_buffer;
  return fb;
}

sceneContext* newSceneContext() {
	sceneContext* sc = malloc(sizeof(sceneContext));
	sc->no_of_objs = 0;
	sc->objs = NULL;
	return sc;
}

int destroyFrameBuffer(frameBuffer *fb) {

	if (fb == NULL) {
		return -1;
	}

  free(fb->buffer);
  if (fb->depth_buffer) {
    free(fb->depth_buffer);
  }
  free(fb);
  return 0;
}

int destroyContext(renderContext* rc) {

	if ( rc == NULL ) {
		return -1;
	}
	if (rc->scene_context == NULL) {
		destroyFrameBuffer(rc->frame_buffer);
		free(rc);
	}else {
		sceneContext* sc = rc->scene_context;
		for (int i=0;i<sc->no_of_objs;i++) {
			free(sc->objs);
		}

		destroyFrameBuffer(rc->frame_buffer);
		free(rc);
		free(sc);
	}
  return 0;
}

int flushPixelBuffer(pixelBuffer *pb, int width, int height) {
	memset((void *)pb, 0, height * width * sizeof(pixelBuffer));
	return 0;
}

int flushDepthBuffer(int *depth_buffer, int width, int height) {
    int total = width * height;
    if (depth_buffer != NULL) {
        for (int i = 0; i < total; i++) {
            depth_buffer[i] = INT_MAX;
        }
    }
    return 0;
}

int flushSceneContext(sceneContext *sc){
	if (sc != NULL) {
		sc->no_of_objs = 0;
	}
	return 0;
}


void formatBuffer(frameBuffer *fb) {
	for (int j = 0; j < fb->width; j++) {
		for (int i = 0; i < fb->height; i++) {
			if (fb->buffer[j * fb->width + i].color.literal) {
				uint16_t literal = fb->buffer[(j * (fb->width)) + i].color.literal;
				int r = ((literal & 0x0F80 ) >> 11 ),
					 g= ((literal & 0x07E0) >> 5 ),
					 b = (literal  & 0x001F);
				r = (r << 3) | (r >> 2);
				g = (g << 2) | (g >> 4);
				b = (b << 3) | (b >> 2);
				printf("\033[38;2;%d;%d;%dmX\033[0m\t", r,g,b);
			} else {
				printf(".\t");
			}
		}
		printf("\n");
	}
	printf("\x1B[2J");
	printf("\x1B[H");
}

int drawPixel(renderContext *rc, int x, int y, Color color) {
  int width = rc->frame_buffer->width;
  int height = rc->frame_buffer->height;

  if (x >= width || y >= height || x < 0 || y < 0) {
    return -1;
  }

  int idx = get_index(rc->frame_buffer, x, y);
  rc->frame_buffer->buffer[idx].color = color;
  if (rc->frame_buffer->depth_buffer) {
      rc->frame_buffer->depth_buffer[idx] = 0;
  }
  return 0;
}

static int obj_to_world(float v, float scale, int offset) {
    return (int)lroundf(v * scale) + offset;
}

void renderMesh3D(renderContext *rc, fastObjMesh *mesh, Point3 offset, Color color, float scale) {
    unsigned int idx_offset = 0;

	 int has_materials = (mesh->material_count > 0 && mesh->face_materials != NULL);
    for (unsigned int f = 0; f < mesh->face_count; f++) {
        unsigned int fv = mesh->face_vertices[f];

		  Color face_color = color;

		  if (has_materials) {
			  unsigned int mat_idx = mesh->face_materials[f];
			  if (mat_idx < mesh->material_count) {
				  face_color = kd_to_color(mesh->materials[mat_idx].Kd);
			  }
		  }

        for (unsigned int i = 1; i + 1 < fv; i++) {
            fastObjIndex i0 = mesh->indices[idx_offset];
            fastObjIndex i1 = mesh->indices[idx_offset + i];
            fastObjIndex i2 = mesh->indices[idx_offset + i + 1];

            Point3 p0 = { obj_to_world(mesh->positions[3 * i0.p + 0], scale, offset.x),
                          obj_to_world(mesh->positions[3 * i0.p + 1], scale, offset.y),
                          obj_to_world(mesh->positions[3 * i0.p + 2], scale, offset.z) };
            Point3 p1 = { obj_to_world(mesh->positions[3 * i1.p + 0], scale, offset.x),
                          obj_to_world(mesh->positions[3 * i1.p + 1], scale, offset.y),
                          obj_to_world(mesh->positions[3 * i1.p + 2], scale, offset.z) };
            Point3 p2 = { obj_to_world(mesh->positions[3 * i2.p + 0], scale, offset.x),
                          obj_to_world(mesh->positions[3 * i2.p + 1], scale, offset.y),
                          obj_to_world(mesh->positions[3 * i2.p + 2], scale, offset.z) };

            Point3 *tri[3] = { &p0, &p1, &p2 };
            renderTriangle3D(rc, tri, face_color);
        }

        idx_offset += fv;
    }
}
