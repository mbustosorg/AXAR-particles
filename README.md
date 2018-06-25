## Dependencies

https://pocoproject.org/download/ - This library provides a variety of utilities for HTTPS, file access and JSON processing.

https://libcinder.org/download - This library is the OpenGL implementation used for rendering.

http://slproweb.com/products/Win32OpenSSL.html - These are the OpenSSL dependencies needed by POCO

## Environment Variables

The system relies on two environment variables, the GOOGLE_PLACES_API_KEY and DATA_ROOT.  The api key is used to help identify headquarter addresses for placement on the Geographic screen.  The data root variable specifies the location of the data JSON files.

```
$ export GOOGLE_PLACES_API_KEY=****
$ export DATA_ROOT=****
```

