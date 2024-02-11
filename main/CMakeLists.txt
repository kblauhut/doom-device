idf_component_register(SRCS 
                        "main.cpp" 
                        "game/map.cpp" 
                        "game/player.cpp"
                        "game/crosshair.cpp"
                        "game/rail.cpp"
                        "io/display.c"
                        "io/controller.cpp"
                        "math/intersect.cpp"
                        "math/math.cpp"
                        "render/frame_renderer.cpp"
                        "render/portal_renderer.cpp"
                        "render/rasterizer.cpp"
                        "render/render_util.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES lvgl
                    )