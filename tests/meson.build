criterion = libcriterion.get_variable('criterion')

test_stream = executable('test_stream', 'test_stream.c', 
                          dependencies: [criterion, nclib],
                          include_directories: incdir)
test('Test stream.', test_stream)

test_mutable_stream = executable('test_mutable_stream', 'test_mutable_stream.c', 
                          dependencies: [criterion, nclib],
                          include_directories: incdir)
test('Test mutable stream.', test_mutable_stream)

test_arena = executable('test_arena', 'test_arena.c', 
                          dependencies: [criterion, nclib],
                          include_directories: incdir)
test('Test arena.', test_arena)
