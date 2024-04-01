project('pong', 'c', default_options : ['c_std=c2x', 'warning_level=3'])

main = files('src/main.c')
executable('pong', main,
  dependencies: [
    dependency('raylib', required: true),
  ],
  c_args : ['-march=native', '-mtune=native'],
  install: true
)

