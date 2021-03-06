{
  'variables': {
    'conditions': [
    ],
  },
  'targets': [
    {
      'target_name': 'comm_static',
      'type': 'static_library',
      'defines': [],
      'variables': {
        'chromium_code': 0,
	    'msvs_use_common_release': 0,
      },
      'dependencies': [
      ],
      'include_dirs': [
      ],
      'sources': [
		'alloc/alloc.h',
		'alloc/alloc.cpp',
		'alloc/allocrp.h',
		'alloc/chunkalloc.h',
		'alloc/chunkblock.h',
		'alloc/chunkpage.h',
		'alloc/chunkpool.h',
		'alloc/commalloc.h',
		'alloc/memtrack.h',
		'alloc/memtrack.cpp',
		'alloc/slotalloc.h',
		'alloc/sseg.h',
		'alloc/sseg.cpp',
		'alloc/_malloc.h',
		'alloc/_malloc.c',
		'atomic/atomic.h',
		'atomic/atomic.cpp',
		'atomic/basic_pool.h',
		'atomic/pool.h',
		'atomic/pool_base.h',
		'atomic/queue.h',
		'atomic/queue_base.h',
		'atomic/stack.h',
		'atomic/stack_base.h',
		'binstream/binstream.h',
		'binstream/binstreambuf.h',
		'binstream/binstreamsegbuf.h',
		'binstream/bstype.h',
		'binstream/cachestream.h',
		'binstream/circularstreambuf.h',
		'binstream/container.h',
		'binstream/container_linear.h',
		'binstream/enc_base64stream.h',
		'binstream/enc_hexstream.h',
		'binstream/exestream.h',
		'binstream/filestream.h',
		'binstream/filestreamgz.h',
		'binstream/forkstream.h',
		'binstream/hash_sha1stream.h',
		'binstream/httpstream.h',
		'binstream/httpstreamcoid.h',
		'binstream/httpstreamtunnel.h',
		'binstream/inoutstream.h',
		'binstream/netstream.h',
		'binstream/netstreamcoid.h',
		'binstream/netstreamhttp.h',
		'binstream/netstreamtcp.h',
		'binstream/netstreamudp.h',
		'binstream/netstreamudpbuf.h',
		'binstream/nullstream.h',
		'binstream/packstream.h',
		'binstream/packstreambzip2.h',
		'binstream/packstreamlz4.h',
		'binstream/packstreamlzo.h',
		'binstream/packstreamzip.h',
		#'binstream/sfstream.h',
		#'binstream/sfstream.cpp',
		'binstream/stdstream.h',
		'binstream/stlstream.h',
		'binstream/textstream.h',
		'binstream/txtcachestream.h',
		'binstream/txtstream.h',
		'binstream/txtstreamhtml.h',
		'coder/lz4/lz4.h',
		'coder/lz4/lz4hc.h',
		'coder/lz4/xxhash.h',
		'coder/rlr.h',
		#'comm_test/atomic-test.cpp',
		#'comm_test/main.cpp',
		#'comm_test/malloc.cpp',
		#'comm_test/meta.cpp',
		#'comm_test/meta2.cpp',
		#'comm_test/meta3.cpp',
		#'comm_test/regex.cpp',
		#'comm_test/stream.cpp',
		'crypt/sha1.h',
		'crypt/sha1.cpp',
		'hash/hashfunc.h',
		'hash/hashkeyset.h',
		'hash/hashmap.h',
		'hash/hashset.h',
		'hash/hashtable.h',
		'intergen/ifc.h',
		'intergen/ifc.js.h',
		'intergen/ig.h',
		#'intergen/test/ifc/thingface.h',
		#'intergen/test/ifc/thingface.js.h',
		'intergen/ig-class.cpp',
		'intergen/ig-lexer.cpp',
		'intergen/ig-method.cpp',
		'intergen/intergen.cpp',
		'intergen/rl-method.cpp',
		#'intergen/test/test.intergen.cpp',
		#'intergen/test/test.intergen.js.cpp',
		'log/logger.h',
		'log/logger.cpp',
		'log/logwriter.h',
		'log/policy_log.h',
		'log/policy_log.cpp',
		'metastream/fmtstream.h',
		'metastream/fmtstreamcxx.h',
		'metastream/fmtstreamjson.h',
		'metastream/fmtstreamnull.h',
		'metastream/fmtstreamxml.h',
		'metastream/fmtstreamxml2.h',
		'metastream/fmtstream_lexer.h',
		'metastream/fmtstream_v8.h',
		'metastream/metagen.h',
		'metastream/metastream.h',
		'metastream/metastream.cpp',
		'metastream/metavar.h',
		'net/asyncrcv.h',
		'net/asyncrcv.cpp',
		'regex/regcomp.h',
		'regex/regcomp.cpp',
		'regex/regexec.cpp',
		'sync/guard.h',
		'sync/mutex.h',
		'sync/mutex.cpp',
		'sync/mutex_reg.h',
		'sync/rw_mutex.h',
		'sync/rw_mx_core.h',
		'sync/thread_mgr.h',
		'sync/_mutex.h',
		'sync/_mutex.cpp',
		'sync/rw_mx_core.cpp',
		'sync/rw_mx_core_pthread.cpp',
		'sync/thread_mgr.cpp',
		#'tests/tests.h',
		#'tests/tests.cpp',
		'namespace.h',
		'net.h',
		'binstring.h',
		'comm.h',
		'comm.cpp',
		'commassert.h',
		'assert.cpp',
		'commexception.h',
		'commtime.h',
		'commtypes.h',
		'dbg_location.h',
		'dir.h',
		'dir.cpp',
		'dirgcc.cpp',
		'dirwin.cpp',
		'dynarray.h',
		'fastdelegate.h',
		'timer.h',
		'timer.cpp',
		#'hptimeru.cpp',
		'interface.h',
		'interface.cpp',
		'lexer.h',
		'list.h',
		'local.h',
		'mathf.h',
		'mathi.h',
		'net_ul.h',
		'net.cpp',
		'parser.h',
		'password.h',
		'pthreadx.h',
		'pthreadx.cpp',
		'ptrowner.h',
		'radix.h',
		'ref.h',
		'refs.h',
		'ref_base.h',
		'ref_helpers.h',
		'ref_i.h',
		'ref_s.h',
		'regex.h',
		'retcodes.h',
		'retcodes.cpp',
		'rnd.h',
		'roundbuffer.h',
		'segarray.h',
		'singleton.h',
		'singleton.cpp',
		'str.h',
		'str-win.cpp',
		'strgen.h',
		'substring.h',
		'substring.cpp',
		'token.h',
		'tokenizer.h',
		'trait.h',
		'tutf8.h',
		'txtconv.h',
		'txtconv.cpp',
		'version.h',
      ],
      'msvs_settings': {
        'VCLinkerTool': {
          'SubSystem': '2',  # Set /SUBSYSTEM:WINDOWS
        },
      },
      'conditions': [
	  ],
	  'configurations': {
        'Debug_Base': {
	      'msvs_configuration_attributes': {
	        'CharacterSet': '2',
	      },
          'msvs_settings': {
            'VCLinkerTool': {
              'LinkIncremental': '<(msvs_large_module_debug_link_mode)',
            },
          },
		  'VCCLCompilerTool': {
            'ExceptionHandling': '1', # /EHsc
            'AdditionalOptions': [
              '/MP',
            ],
          },          
        },
      },
    },
  ]
}
