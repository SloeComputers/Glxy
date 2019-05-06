
source   = ['Source/KeplerianElements.cpp']
# binaries = ['glxy', 'dbg', 'stars']
binaries = ['glxy']
app      = 'Glxy'
version  = '0.0.0'

# Get a build environment
env,libs = SConscript('Platform/build.scons', ['app', 'version'])

# Project specific build config
env.Append(CCFLAGS = ['-O3', '-g'])

# Builders
exe = []
for binary in binaries:
   exe += env.Program(binary, ['Source/'+binary+'.cpp'] + source + env['startup'])
Depends(exe, libs)

env.Tar(app+'_'+env['target']+'_'+env['machine']+'_'+version+'.tgz',
        [exe, env['platform_files'], 'LICENSE'])

