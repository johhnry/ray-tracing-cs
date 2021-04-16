sources = files(
  'log/Log.cpp',

  'texture/Texture.cpp',
  'geometry/FullScreenVAO.cpp',

  'shader/IShader.cpp',
  'shader/FragmentShader.cpp',
  'shader/VertexShader.cpp',
  'shader/ComputeShader.cpp',
  'shader/Program.cpp',
  'shader/ComputeProgram.cpp',

  'app/ApplicationWindow.cpp',
  'app/Application.cpp',

  'main.cpp'
)

dependencies = [
  dependency('gl'),
  dependency('glew'),
  dependency('glfw3'),
]

main = executable(
  'ray-tracing-cs',
  sources,
  dependencies : dependencies
)
