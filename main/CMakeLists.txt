idf_component_register(SRCS "main.cpp" 
                        "game/map.cpp" 
                        "game/player.cpp"
                        "io/display.c"
                        "math/intersect.cpp"
                        "math/math.cpp"
                        "render/frame_renderer.cpp"
                        "render/portal_renderer.cpp"
                        "render/rasterizer.cpp"
                        "render/render_util.cpp"
                    INCLUDE_DIRS "."
                    REQUIRES lvgl
                    )