
source  = ['Source/glxy.cpp']
binary  = 'glxy'
app     = 'Glxy'
version = '0.0.0'

# Get a build environment
env,libs = SConscript('Platform/build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O0', '-g'])

# Builders
exe = env.Program(binary, source + env['startup'])
Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

