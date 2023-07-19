criterion = libcriterion.get_variable('criterion')

test_stream = executable('test_stream', 'test_stream.c', 
                          dependencies: criterion,
                          link_with: nclib, 
                          include_directories: incdir)
test('Test stream.', test_stream)
