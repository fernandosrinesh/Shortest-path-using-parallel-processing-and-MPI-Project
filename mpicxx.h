#ifdef MPI
#error "You cannot define MPI; that name is reserved for the MPI namespace"
#endif

// Check for incompatible GCC versions
// GCC (specifically) g++ changed the calling convention
// between 3.2.3 and 3.4.3 (!!)  Normally such changes
// should only occur at major releases (e.g., version 3 to 4)
#ifdef __GNUC__ 
# if __GNUC__ >= 4 
#  if __GNUC_MINOR__ > 2 && 8 == 2 
#  error 'Please use the same version of GCC and g++ for compiling MPICH and user MPI programs'
#  endif
# endif     
#endif

/* 
 * Because the API is defined, some methods have parameters that are 
 * not used.  The following definition allows us to suppress warnings
 * about unused arguments in methods when -Wall -Wextra are specified.
 * this definition is removed at the end of this file.
 */
#ifdef MPIR_ARGUNUSED
#error MPIR_ARGUNUSED defined
#endif
#if defined(__GNUC__) && __GNUC__ >= 4 
#define MPIR_ARGUNUSED __attribute__((unused))
#else
#define MPIR_ARGUNUSED
#endif
// There is a name conflict between stdio.h and iostream (or iostream.h)
// and the MPI C++ binding with respect to the names SEEK_SET, SEEK_CUR, 
// and SEEK_END.  MPI wants these in the MPI namespace, but stdio.h, 
// iostream, or iostream.h will #define these to integer values.  
// #undef'ing these can cause obscure problems.  
#ifndef MPICH_IGNORE_CXX_SEEK

// MPICH_DONT_INCLUDE_STDIO_H is another escape hatch for us, just like
// MPICH_IGNORE_CXX_SEEK.  If we encounter a wacky environment or user in the
// wild that does not want our workaround and/or the stdio.h header, then we can
// offer them a way out.
#ifndef MPICH_DONT_INCLUDE_STDIO_H
// ensure that we have SEEK_* defined
# include <stdio.h>
#endif

enum MPIR_Dummy_seek_type {
    MPIR_DUMMY_SEEK_COMMA_VAL = -1  // permits cleaner comma logic
#ifdef SEEK_SET
    , MPIR_SEEK_SET = SEEK_SET
#   undef SEEK_SET
    , SEEK_SET = MPIR_SEEK_SET
#endif
#ifdef SEEK_CUR
    , MPIR_SEEK_CUR = SEEK_CUR
#   undef SEEK_CUR
    , SEEK_CUR = MPIR_SEEK_CUR
#endif
#ifdef SEEK_END
    , MPIR_SEEK_END = SEEK_END
#   undef SEEK_END
    , SEEK_END = MPIR_SEEK_END
#endif
#ifdef LOCK_SHARED
    , MPIR_LOCK_SHARED = LOCK_SHARED
#   undef LOCK_SHARED
    , LOCK_SHARED = MPIR_LOCK_SHARED
#endif
};

#endif // MPICH_IGNORE_CXX_SEEK
namespace MPI {
#if 1
#define MPIX_CALLREF( _objptr, fnc ) \
    { int err = fnc; if (err) { (_objptr)->Call_errhandler( err ); }}
#define MPIX_CALLOBJ( _obj, fnc ) \
    { int err = fnc; if (err) { (_obj).Call_errhandler( err ); }}
#define MPIX_CALLWORLD( fnc ) \
    { int err = fnc ; if (err) MPIR_Call_world_errhand( err ); }
extern void MPIR_Call_world_errhand( int );
#else
#define MPIX_CALLREF( _objptr, fnc ) (void)fnc
#define MPIX_CALLOBJ( _obj, fnc ) (void)fnc
#define MPIX_CALLWORLD( fnc ) (void)fnc
#endif

// Typedefs for basic int types
typedef MPI_Offset Offset;
typedef MPI_Aint   Aint;
typedef MPI_Fint   Fint;

// Special internal routine
void MPIR_CXX_InitDatatypeNames( void );

// Forward class declarations
class Comm;
class Nullcomm;
class Intercomm;
class Intracomm;
class Cartcomm;
class Graphcomm;
class File;

// base (classless) routines
extern     int Detach_buffer( void *&v1 ) ;
extern     bool Is_initialized( void ) ;
extern     void Get_processor_name( char * v1, int &v2 ) ;
extern     void Get_error_string( int v1, char * v2, int &v3 ) ;
extern     void Compute_dims( int v1, int v2, int v3[] ) ;
extern     void Get_version( int &v1, int &v2 ) ;
extern     void Finalize( void ) ;
extern     void Pcontrol( const int v1, ... ) ;
extern     void Attach_buffer( void * v1, int v2 ) ;
extern     int Get_error_class( int v1 ) ;
extern     Intracomm COMM_WORLD;
extern     File FILE_NULL;

class Exception  {

  protected:
    int the_real_exception;

  public:
    // new/delete

    inline Exception(int obj) : the_real_exception(obj) {}
    inline Exception(void) : the_real_exception(0) {}

    virtual ~Exception() {}
    // copy/assignment

    Exception(const Exception &obj) : the_real_exception(obj.the_real_exception){}

    Exception& operator=(const Exception &obj) {
      the_real_exception = obj.the_real_exception; return *this; }

    // logical
    bool operator== (const Exception &obj) {
      return (the_real_exception == obj.the_real_exception); }
    bool operator!= (const Exception &obj) {
      return (the_real_exception != obj.the_real_exception); }
    // C/C++ cast and assignment
    inline operator int*() { return &the_real_exception; }
    inline operator int() const { return the_real_exception; }
    Exception& operator=(const int& obj) {
      the_real_exception = obj; return *this; }

  protected:
    char the_error_message[MPI_MAX_ERROR_STRING];
  public:
    int Get_error_code(void) { return the_real_exception; } 
    int Get_error_class(void) { return MPI::Get_error_class(the_real_exception); } 
    const char *Get_error_string(void)
    {
	int len;
	MPI_Error_string(the_real_exception, the_error_message, &len);
	return the_error_message;
    }
};

class Datatype  {
    friend class Comm;
    friend class Status;
    friend class Intracomm;
    friend class Intercomm;
    friend class Win;
    friend class File;
    friend class Op;

  protected:
    MPI_Datatype the_real_datatype;

  public:
    // new/delete

    inline Datatype(MPI_Datatype obj) : the_real_datatype(obj) {}
    inline Datatype(void) : the_real_datatype(MPI_DATATYPE_NULL) {}

    virtual ~Datatype() {}
    // copy/assignment

    Datatype(const Datatype &obj) : the_real_datatype(obj.the_real_datatype){}

    Datatype& operator=(const Datatype &obj) {
      the_real_datatype = obj.the_real_datatype; return *this; }

    // logical
    bool operator== (const Datatype &obj) {
      return (the_real_datatype == obj.the_real_datatype); }
    bool operator!= (const Datatype &obj) {
      return (the_real_datatype != obj.the_real_datatype); }
    // C/C++ cast and assignment
    inline operator MPI_Datatype*() { return &the_real_datatype; }
    inline operator MPI_Datatype() const { return the_real_datatype; }
    Datatype& operator=(const MPI_Datatype& obj) {
      the_real_datatype = obj; return *this; }
    virtual void Commit( void ) 
    {
        MPIX_CALLWORLD( MPI_Type_commit( (MPI_Datatype *) &the_real_datatype ));
    }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Type_free( (MPI_Datatype *) &the_real_datatype ));
    }
    virtual Datatype Create_indexed( int v1, const int * v2, const int * v3 ) const
    {
        Datatype v5;
        MPIX_CALLWORLD( MPI_Type_indexed( v1, (const int *)v2, (const int *)v3, (MPI_Datatype) the_real_datatype, &(v5.the_real_datatype) ));
        return v5;
    }
    virtual Datatype Create_contiguous( int v1 ) const
    {
        Datatype v3;
        MPIX_CALLWORLD( MPI_Type_contiguous( v1, (MPI_Datatype) the_real_datatype, &(v3.the_real_datatype) ));
        return v3;
    }
    virtual Datatype Create_vector( int v1, int v2, int v3 ) const
    {
        Datatype v5;
        MPIX_CALLWORLD( MPI_Type_vector( v1, v2, v3, (MPI_Datatype) the_real_datatype, &(v5.the_real_datatype) ));
        return v5;
    }
    static Datatype Create_struct( int v1, int v2[], Aint v3[],  const Datatype v4[] ) 
    {
        Datatype v5;
        MPI_Datatype *l4 = new MPI_Datatype[v1];
        { 
            int i4; 
            for (i4=0;i4<v1;i4++) {
                l4[i4] = v4[i4].the_real_datatype;
            }
        }
        MPIX_CALLWORLD( MPI_Type_create_struct( v1, (const int  *)v2, (const MPI_Aint  *)v3, l4, &(v5.the_real_datatype) ));
                    delete[] l4;
        return v5;
    }
    virtual int Pack_size( int v1, const Comm &v3 ) const;
    virtual void Pack( const void * v1, int v2, void * v4, int v5, int &v6, const Comm &v7 ) const;
    virtual int Get_size( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Type_size( (MPI_Datatype) the_real_datatype, &v2 ));
        return v2;
    }
    virtual void Get_envelope( int &v2, int &v3, int &v4, int &v5 ) const
    {
        MPIX_CALLWORLD( MPI_Type_get_envelope( (MPI_Datatype) the_real_datatype, &v2, &v3, &v4, &v5 ));
    }
    virtual Datatype Create_hvector( int v1, int v2, Aint v3 ) const
    {
        Datatype v5;
        MPIX_CALLWORLD( MPI_Type_create_hvector( v1, v2, v3, (MPI_Datatype) the_real_datatype, &(v5.the_real_datatype) ));
        return v5;
    }
    static Datatype Match_size( int v1, int v2 ) 
    {
        Datatype v3;
        MPIX_CALLWORLD( MPI_Type_match_size( v1, v2, &(v3.the_real_datatype) ));
        return v3;
    }
    virtual Datatype Create_resized( const Aint v2, const Aint v3 ) const
    {
        Datatype v4;
        MPIX_CALLWORLD( MPI_Type_create_resized( (MPI_Datatype) the_real_datatype, (MPI_Aint)v2, (MPI_Aint)v3, &(v4.the_real_datatype) ));
        return v4;
    }
    virtual Datatype Create_indexed_block( int v1, int v2, const int v3[] ) const
    {
        Datatype v5;
        MPIX_CALLWORLD( MPI_Type_create_indexed_block( v1, v2, (const int  *)v3, (MPI_Datatype) the_real_datatype, &(v5.the_real_datatype) ));
        return v5;
    }
    virtual Aint Pack_external_size( const char v1[], int v2 ) const
    {
        MPI_Aint v4;
        MPIX_CALLWORLD( MPI_Pack_external_size( v1, v2, (MPI_Datatype) the_real_datatype, &v4 ));
        return v4;
    }
    Datatype Dup( void ) const
    {
        Datatype v2;
        MPIX_CALLWORLD( MPI_Type_dup( (MPI_Datatype) the_real_datatype, &(v2.the_real_datatype) ));
        return v2;
    }
    virtual Datatype Create_hindexed( int v1, const int v2[], const Aint v3[] ) const
    {
        Datatype v5;
        MPIX_CALLWORLD( MPI_Type_create_hindexed( v1, (const int  *)v2, (const MPI_Aint  *)v3, (MPI_Datatype) the_real_datatype, &(v5.the_real_datatype) ));
        return v5;
    }
    virtual bool Get_attr( int v2, void * v3 ) const
    {
        int v4;
        MPIX_CALLWORLD( MPI_Type_get_attr( (MPI_Datatype) the_real_datatype, v2, v3, &v4 ));
        return v4!= 0;
    }
    virtual void Get_true_extent( Aint & v2, Aint & v3 ) const
    {
        MPIX_CALLWORLD( MPI_Type_get_true_extent( (MPI_Datatype) the_real_datatype, &v2, &v3 ));
    }
    virtual Datatype Create_darray( int v1, int v2, int v3, const int v4[], const int v5[], const int v6[], const int v7[], int v8 ) const
    {
        Datatype v10;
        MPIX_CALLWORLD( MPI_Type_create_darray( v1, v2, v3, (const int  *)v4, (const int  *)v5, (const int  *)v6, (const int  *)v7, v8, (MPI_Datatype) the_real_datatype, &(v10.the_real_datatype) ));
        return v10;
    }
    static Datatype Create_f90_real( int v1, int v2 ) 
    {
        Datatype v3;
        MPIX_CALLWORLD( MPI_Type_create_f90_real( v1, v2, &(v3.the_real_datatype) ));
        return v3;
    }
    virtual void Get_contents( int v2, int v3, int v4, int v5[], Aint v6[], Datatype v7[] ) const
    {
        MPI_Datatype *l7 = new MPI_Datatype[v4];
        MPIX_CALLWORLD( MPI_Type_get_contents( (MPI_Datatype) the_real_datatype, v2, v3, v4, v5, v6, l7 ));
        { 
            int i7; 
            for (i7=0;i7<v4;i7++) {
                v7[i7].the_real_datatype = l7[i7];
            }
            delete[] l7;
        }
    }
    virtual void Set_attr( int v2, const void * v3 ) 
    {
        MPIX_CALLWORLD( MPI_Type_set_attr( (MPI_Datatype) the_real_datatype, v2, (void *)v3 ));
    }
    virtual void Set_name( const char * v2 ) 
    {
        MPIX_CALLWORLD( MPI_Type_set_name( (MPI_Datatype) the_real_datatype, (const char *)v2 ));
    }
    static Datatype Create_f90_complex( int v1, int v2 ) 
    {
        Datatype v3;
        MPIX_CALLWORLD( MPI_Type_create_f90_complex( v1, v2, &(v3.the_real_datatype) ));
        return v3;
    }
    virtual Datatype Create_subarray( int v1, const int v2[], const int v3[], const int v4[], const int v5 ) const
    {
        Datatype v7;
        MPIX_CALLWORLD( MPI_Type_create_subarray( v1, v2, (const int  *)v3, (const int  *)v4, (int)v5, (MPI_Datatype) the_real_datatype, &(v7.the_real_datatype) ));
        return v7;
    }
    virtual void Unpack_external( const char v1[], const void * v2, const Aint v3, Aint & v4, void * v5, int v6 ) const
    {
        MPIX_CALLWORLD( MPI_Unpack_external( v1, (const void *)v2, (MPI_Aint)v3, &v4, v5, v6, (MPI_Datatype) the_real_datatype ));
    }
    static void Free_keyval( int &v1 ) 
    {
        MPIX_CALLWORLD( MPI_Type_free_keyval( &v1 ));
    }
    static Datatype Create_struct( int v1, const int v2[], const Aint v3[],  const Datatype v4[] ) 
    {
        Datatype v5;
        MPI_Datatype *l4 = new MPI_Datatype[v1];
        { 
            int i4; 
            for (i4=0;i4<v1;i4++) {
                l4[i4] = v4[i4].the_real_datatype;
            }
        }
        MPIX_CALLWORLD( MPI_Type_create_struct( v1, (const int  *)v2, (const MPI_Aint  *)v3, l4, &(v5.the_real_datatype) ));
                    delete[] l4;
        return v5;
    }
    static Datatype Create_f90_integer( int v1 ) 
    {
        Datatype v2;
        MPIX_CALLWORLD( MPI_Type_create_f90_integer( v1, &(v2.the_real_datatype) ));
        return v2;
    }
    virtual void Pack_external( const char v1[], const void * v2, const int v3, void * v5, Aint v6, Aint & v7 ) const
    {
        MPIX_CALLWORLD( MPI_Pack_external( v1, (const void *)v2, (int)v3, (MPI_Datatype) the_real_datatype, v5, v6, &v7 ));
    }
    virtual void Get_extent( Aint & v2, Aint & v3 ) const
    {
        MPIX_CALLWORLD( MPI_Type_get_extent( (MPI_Datatype) the_real_datatype, &v2, &v3 ));
    }
    virtual void Delete_attr( int v2 ) 
    {
        MPIX_CALLWORLD( MPI_Type_delete_attr( (MPI_Datatype) the_real_datatype, v2 ));
    }
    virtual void Get_name( char * v2, int &v3 ) const
    {
    MPIR_CXX_InitDatatypeNames();
        MPIX_CALLWORLD( MPI_Type_get_name( (MPI_Datatype) the_real_datatype, v2, &v3 ));
    }

    void Unpack( const void *, int, void *, int, int &, const Comm & ) const;
    typedef int Copy_attr_function(const Datatype& oldtype, int type_keyval, void* extra_state, void* attribute_val_in, void* attribute_val_out, bool& flag); 
    typedef int Delete_attr_function(Datatype& type, int type_keyval, void* attribute_val, void* extra_state); 

    static int Create_keyval( Copy_attr_function *, Delete_attr_function *,
                              void * );
    // These functions are *not* part of MPI-2 but are provided
    // because they should have been included
    static int NULL_COPY_FN( const Datatype &oldtype MPIR_ARGUNUSED, 
        int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
        void *attr_in MPIR_ARGUNUSED, void *attr_out MPIR_ARGUNUSED, 
        bool &flag ) { flag = 1; return 0;}
    static int NULL_DELETE_FN( Datatype &type MPIR_ARGUNUSED, 
        int keyval MPIR_ARGUNUSED, void * attr MPIR_ARGUNUSED, 
        void *ex MPIR_ARGUNUSED ) { return 0; }
    static int DUP_FN( const Datatype &oldtype MPIR_ARGUNUSED, 
        int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
        void *attr_in, void *attr_out, bool &flag ) { flag = 1; 
            *(void **)attr_out = attr_in; return 0;} 
          
};

    typedef void User_function(const void *, void*, int, const Datatype&); 

class Info  {
    friend class File;
    friend class Win;
    friend class Comm;
    friend class Intracomm;

  protected:
    MPI_Info the_real_info;

  public:
    // new/delete

    inline Info(MPI_Info obj) : the_real_info(obj) {}
    inline Info(void) : the_real_info(MPI_INFO_NULL) {}

    virtual ~Info() {}
    // copy/assignment

    Info(const Info &obj) : the_real_info(obj.the_real_info){}

    Info& operator=(const Info &obj) {
      the_real_info = obj.the_real_info; return *this; }

    // logical
    bool operator== (const Info &obj) {
      return (the_real_info == obj.the_real_info); }
    bool operator!= (const Info &obj) {
      return (the_real_info != obj.the_real_info); }
    // C/C++ cast and assignment
    inline operator MPI_Info*() { return &the_real_info; }
    inline operator MPI_Info() const { return the_real_info; }
    Info& operator=(const MPI_Info& obj) {
      the_real_info = obj; return *this; }
    virtual void Delete( const char * v2 ) 
    {
        MPIX_CALLWORLD( MPI_Info_delete( (MPI_Info) the_real_info, (const char *)v2 ));
    }
    virtual void Get_nthkey( int v2, char * v3 ) const
    {
        MPIX_CALLWORLD( MPI_Info_get_nthkey( (MPI_Info) the_real_info, v2, v3 ));
    }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Info_free( (MPI_Info *) &the_real_info ));
    }
    static Info Create( void ) 
    {
        Info v1;
        MPIX_CALLWORLD( MPI_Info_create( &(v1.the_real_info) ));
        return v1;
    }
    virtual void Set( const char * v2, const char * v3 ) 
    {
        MPIX_CALLWORLD( MPI_Info_set( (MPI_Info) the_real_info, (const char *)v2, (const char *)v3 ));
    }
    Info Dup( void ) const
    {
        Info v2;
        MPIX_CALLWORLD( MPI_Info_dup( (MPI_Info) the_real_info, &(v2.the_real_info) ));
        return v2;
    }
    virtual bool Get_valuelen( const char * v2, int &v3 ) const
    {
        int v4;
        MPIX_CALLWORLD( MPI_Info_get_valuelen( (MPI_Info) the_real_info, (const char *)v2, &v3, &v4 ));
        return v4!= 0;
    }
    virtual bool Get( const char * v2, int v3, char * v4 ) const
    {
        int v5;
        MPIX_CALLWORLD( MPI_Info_get( (MPI_Info) the_real_info, (const char *)v2, v3, v4, &v5 ));
        return v5!= 0;
    }
    virtual int Get_nkeys( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Info_get_nkeys( (MPI_Info) the_real_info, &v2 ));
        return v2;
    }
};

class Status  {
    friend class Comm;
    friend class File;
    friend class Request;

  protected:
    MPI_Status the_real_status;

  public:
    // new/delete

    inline Status(MPI_Status obj) : the_real_status(obj) {}
    inline Status(void) : the_real_status() {}

    virtual ~Status() {}
    // copy/assignment

    Status(const Status &obj) : the_real_status(obj.the_real_status){}

    Status& operator=(const Status &obj) {
      the_real_status = obj.the_real_status; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Status*() { return &the_real_status; }
    inline operator MPI_Status() const { return the_real_status; }
    Status& operator=(const MPI_Status& obj) {
      the_real_status = obj; return *this; }
    virtual bool Is_cancelled( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Test_cancelled( (const MPI_Status *) &the_real_status, &v2 ));
        return v2!= 0;
    }
    virtual int Get_elements( const Datatype &v2 ) const
    {
        int v3;
        MPIX_CALLWORLD( MPI_Get_elements( (const MPI_Status *) &the_real_status, (MPI_Datatype)(v2.the_real_datatype), &v3 ));
        return v3;
    }
    virtual int Get_count( const Datatype &v2 ) const
    {
        int v3;
        MPIX_CALLWORLD( MPI_Get_count( (const MPI_Status *) &the_real_status, (MPI_Datatype)(v2.the_real_datatype), &v3 ));
        return v3;
    }
    virtual void Set_cancelled( bool v2 ) 
    {
        int l2;
         l2 = (v2 == true) ? 1 : 0;
        MPIX_CALLWORLD( MPI_Status_set_cancelled( (MPI_Status *) &the_real_status, l2 ));
    }
    virtual void Set_elements( const Datatype &v2, int v3 ) 
    {
        MPIX_CALLWORLD( MPI_Status_set_elements( (MPI_Status *) &the_real_status, (MPI_Datatype)(v2.the_real_datatype), v3 ));
    }

    int Get_source(void) const { return the_real_status.MPI_SOURCE; }
    int Get_tag(void) const { return the_real_status.MPI_TAG; }
    int Get_error(void) const { return the_real_status.MPI_ERROR; }
    void Set_source(int source) { the_real_status.MPI_SOURCE = source; }
    void Set_tag(int tag) { the_real_status.MPI_TAG = tag; }
    void Set_error(int error) { the_real_status.MPI_ERROR = error; }
};

class Group  {
    friend class Comm;
    friend class Intracomm;
    friend class Intercomm;
    friend class Win;
    friend class File;

  protected:
    MPI_Group the_real_group;

  public:
    // new/delete

    inline Group(MPI_Group obj) : the_real_group(obj) {}
    inline Group(void) : the_real_group(MPI_GROUP_NULL) {}

    virtual ~Group() {}
    // copy/assignment

    Group(const Group &obj) : the_real_group(obj.the_real_group){}

    Group& operator=(const Group &obj) {
      the_real_group = obj.the_real_group; return *this; }

    // logical
    bool operator== (const Group &obj) {
      return (the_real_group == obj.the_real_group); }
    bool operator!= (const Group &obj) {
      return (the_real_group != obj.the_real_group); }
    // C/C++ cast and assignment
    inline operator MPI_Group*() { return &the_real_group; }
    inline operator MPI_Group() const { return the_real_group; }
    Group& operator=(const MPI_Group& obj) {
      the_real_group = obj; return *this; }
    virtual Group Excl( int v2, const int v3[] ) const
    {
        Group v4;
        MPIX_CALLWORLD( MPI_Group_excl( (MPI_Group) the_real_group, v2, (const int  *)v3, &(v4.the_real_group) ));
        return v4;
    }
    virtual int Get_rank( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Group_rank( (MPI_Group) the_real_group, &v2 ));
        return v2;
    }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Group_free( (MPI_Group *) &the_real_group ));
    }
    static Group Union( const Group &v1, const Group &v2 ) 
    {
        Group v3;
        MPIX_CALLWORLD( MPI_Group_union( (MPI_Group)(v1.the_real_group), (MPI_Group)(v2.the_real_group), &(v3.the_real_group) ));
        return v3;
    }
    static Group Intersect( const Group &v1, const Group &v2 ) 
    {
        Group v3;
        MPIX_CALLWORLD( MPI_Group_intersection( (MPI_Group)(v1.the_real_group), (MPI_Group)(v2.the_real_group), &(v3.the_real_group) ));
        return v3;
    }
    virtual Group Range_excl( int v2, const int v3[][3] ) const
    {
        Group v4;
        MPIX_CALLWORLD( MPI_Group_range_excl( (MPI_Group) the_real_group, v2, (int  (*)[3])v3, &(v4.the_real_group) ));
        return v4;
    }
    virtual Group Range_incl( int v2, const int v3[][3] ) const
    {
        Group v4;
        MPIX_CALLWORLD( MPI_Group_range_incl( (MPI_Group) the_real_group, v2, (int  (*)[3])v3, &(v4.the_real_group) ));
        return v4;
    }
    static Group Difference( const Group &v1, const Group &v2 ) 
    {
        Group v3;
        MPIX_CALLWORLD( MPI_Group_difference( (MPI_Group)(v1.the_real_group), (MPI_Group)(v2.the_real_group), &(v3.the_real_group) ));
        return v3;
    }
    static void Translate_ranks( const Group &v1, int v2, const int v3[], const Group &v4, int v5[] ) 
    {
        MPIX_CALLWORLD( MPI_Group_translate_ranks( (MPI_Group)(v1.the_real_group), v2, (const int  *)v3, (MPI_Group)(v4.the_real_group), v5 ));
    }
    virtual Group Incl( int v2, const int v3[] ) const
    {
        Group v4;
        MPIX_CALLWORLD( MPI_Group_incl( (MPI_Group) the_real_group, v2, (const int  *)v3, &(v4.the_real_group) ));
        return v4;
    }
    virtual int Get_size( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Group_size( (MPI_Group) the_real_group, &v2 ));
        return v2;
    }
    static int Compare( const Group &v1, const Group &v2 ) 
    {
        int v3;
        MPIX_CALLWORLD( MPI_Group_compare( (MPI_Group)(v1.the_real_group), (MPI_Group)(v2.the_real_group), &v3 ));
        return v3;
    }
};

class Op  {
    friend class Intracomm;
    friend class Intercomm;
    friend class Win;
    friend class Comm;

  protected:
    MPI_Op the_real_op;

  public:
    // new/delete

    inline Op(MPI_Op obj) : the_real_op(obj) {}
    inline Op(void) : the_real_op(MPI_OP_NULL) {}

    virtual ~Op() {}
    // copy/assignment

    Op(const Op &obj) : the_real_op(obj.the_real_op){}

    Op& operator=(const Op &obj) {
      the_real_op = obj.the_real_op; return *this; }

    // logical
    bool operator== (const Op &obj) {
      return (the_real_op == obj.the_real_op); }
    bool operator!= (const Op &obj) {
      return (the_real_op != obj.the_real_op); }
    // C/C++ cast and assignment
    inline operator MPI_Op*() { return &the_real_op; }
    inline operator MPI_Op() const { return the_real_op; }
    Op& operator=(const MPI_Op& obj) {
      the_real_op = obj; return *this; }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Op_free( (MPI_Op *) &the_real_op ));
    }
    virtual bool Is_commutative( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Op_commutative( (MPI_Op) the_real_op, &v2 ));
        return v2!= 0;
    }
    virtual void Reduce_local( const void * v1, void * v2, int v3, const Datatype &v4 ) const
    {
        MPIX_CALLWORLD( MPI_Reduce_local( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op) the_real_op ));
    }

    void Init( User_function *, bool );
};

class Errhandler  {
    friend class Comm;
    friend class File;
    friend class Win;

  protected:
    MPI_Errhandler the_real_errhandler;

  public:
    // new/delete

    inline Errhandler(MPI_Errhandler obj) : the_real_errhandler(obj) {}
    inline Errhandler(void) : the_real_errhandler(MPI_ERRHANDLER_NULL) {}

    virtual ~Errhandler() {}
    // copy/assignment

    Errhandler(const Errhandler &obj) : the_real_errhandler(obj.the_real_errhandler){}

    Errhandler& operator=(const Errhandler &obj) {
      the_real_errhandler = obj.the_real_errhandler; return *this; }

    // logical
    bool operator== (const Errhandler &obj) {
      return (the_real_errhandler == obj.the_real_errhandler); }
    bool operator!= (const Errhandler &obj) {
      return (the_real_errhandler != obj.the_real_errhandler); }
    // C/C++ cast and assignment
    inline operator MPI_Errhandler*() { return &the_real_errhandler; }
    inline operator MPI_Errhandler() const { return the_real_errhandler; }
    Errhandler& operator=(const MPI_Errhandler& obj) {
      the_real_errhandler = obj; return *this; }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Errhandler_free( (MPI_Errhandler *) &the_real_errhandler ));
    }
};

class Request  {
    friend class Comm;
    friend class File;
    friend class Grequest;

  protected:
    MPI_Request the_real_request;

  public:
    // new/delete

    inline Request(MPI_Request obj) : the_real_request(obj) {}
    inline Request(void) : the_real_request(MPI_REQUEST_NULL) {}

    virtual ~Request() {}
    // copy/assignment

    Request(const Request &obj) : the_real_request(obj.the_real_request){}

    Request& operator=(const Request &obj) {
      the_real_request = obj.the_real_request; return *this; }

    // logical
    bool operator== (const Request &obj) {
      return (the_real_request == obj.the_real_request); }
    bool operator!= (const Request &obj) {
      return (the_real_request != obj.the_real_request); }
    // C/C++ cast and assignment
    inline operator MPI_Request*() { return &the_real_request; }
    inline operator MPI_Request() const { return the_real_request; }
    Request& operator=(const MPI_Request& obj) {
      the_real_request = obj; return *this; }
    static bool Testany( int v1, Request v2[], int &v3, Status & v5 ) 
    {
        int v4;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testany( v1, l2, &v3, &v4, (MPI_Status *)&(v5.the_real_status ) ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v4!= 0;
    }
    static bool Testany( int v1, Request v2[], int &v3 ) 
    {
        int v4;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testany( v1, l2, &v3, &v4, MPI_STATUS_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v4!= 0;
    }
    static int Waitsome( int v1, Request v2[], int v4[], Status v5[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        MPI_Status *l5 = new MPI_Status[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitsome( v1, l2, &v3, v4, l5 ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        { 
            int i5; 
            for (i5=0;i5<v1;i5++) {
                v5[i5].the_real_status = l5[i5];
            }
            delete[] l5;
        }
        return v3;
    }
    static int Waitsome( int v1, Request v2[], int v4[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitsome( v1, l2, &v3, v4, MPI_STATUSES_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v3;
    }
    virtual void Free( void ) 
    {
        MPIX_CALLWORLD( MPI_Request_free( (MPI_Request *) &the_real_request ));
    }
    static bool Testall( int v1, Request v2[], Status v4[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        MPI_Status *l4 = new MPI_Status[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testall( v1, l2, &v3, l4 ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        { 
            int i4; 
            for (i4=0;i4<v1;i4++) {
                v4[i4].the_real_status = l4[i4];
            }
            delete[] l4;
        }
        return v3!= 0;
    }
    static bool Testall( int v1, Request v2[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testall( v1, l2, &v3, MPI_STATUSES_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v3!= 0;
    }
    virtual void Wait( Status & v2 ) 
    {
        MPIX_CALLWORLD( MPI_Wait( (MPI_Request *) &the_real_request, (MPI_Status *)&(v2.the_real_status ) ));
    }
    virtual void Wait( void ) 
    {
        MPIX_CALLWORLD( MPI_Wait( (MPI_Request *) &the_real_request, MPI_STATUS_IGNORE ));
    }
    static int Testsome( int v1, Request v2[], int v4[], Status v5[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        MPI_Status *l5 = new MPI_Status[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testsome( v1, l2, &v3, v4, l5 ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        { 
            int i5; 
            for (i5=0;i5<v1;i5++) {
                v5[i5].the_real_status = l5[i5];
            }
            delete[] l5;
        }
        return v3;
    }
    static int Testsome( int v1, Request v2[], int v4[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Testsome( v1, l2, &v3, v4, MPI_STATUSES_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v3;
    }
    static void Waitall( int v1, Request v2[], Status v3[] ) 
    {
        MPI_Request *l2 = new MPI_Request[v1];
        MPI_Status *l3 = new MPI_Status[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitall( v1, l2, l3 ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        { 
            int i3; 
            for (i3=0;i3<v1;i3++) {
                v3[i3].the_real_status = l3[i3];
            }
            delete[] l3;
        }
    }
    static void Waitall( int v1, Request v2[] ) 
    {
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitall( v1, l2, MPI_STATUSES_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
    }
    static int Waitany( int v1, Request v2[], Status & v4 ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitany( v1, l2, &v3, (MPI_Status *)&(v4.the_real_status ) ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v3;
    }
    static int Waitany( int v1, Request v2[] ) 
    {
        int v3;
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Waitany( v1, l2, &v3, MPI_STATUS_IGNORE ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
        return v3;
    }
    virtual bool Test( Status & v3 ) 
    {
        int v2;
        MPIX_CALLWORLD( MPI_Test( (MPI_Request *) &the_real_request, &v2, (MPI_Status *)&(v3.the_real_status ) ));
        return v2!= 0;
    }
    virtual bool Test( void ) 
    {
        int v2;
        MPIX_CALLWORLD( MPI_Test( (MPI_Request *) &the_real_request, &v2, MPI_STATUS_IGNORE ));
        return v2!= 0;
    }
    virtual void Cancel( void ) const
    {
        MPIX_CALLWORLD( MPI_Cancel( (MPI_Request *) &the_real_request ));
    }
    virtual bool Get_status( Status & v3 ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Request_get_status( (MPI_Request) the_real_request, &v2, (MPI_Status *)&(v3.the_real_status ) ));
        return v2!= 0;
    }
    virtual bool Get_status( void ) const
    {
        int v2;
        MPIX_CALLWORLD( MPI_Request_get_status( (MPI_Request) the_real_request, &v2, MPI_STATUS_IGNORE ));
        return v2!= 0;
    }
};

class Prequest : public Request {

  public:
    // new/delete

    inline Prequest(MPI_Request obj) : Request(obj) {}
    inline Prequest(void) : Request() {}

    virtual ~Prequest() {}
    // copy/assignment

    Prequest(const Prequest &obj) : Request(obj) {}

    Prequest& operator=(const Prequest &obj) {
      the_real_request = obj.the_real_request; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Request*() { return &the_real_request; }
    inline operator MPI_Request() const { return the_real_request; }
    Prequest& operator=(const MPI_Request& obj) {
      the_real_request = obj; return *this; }
    virtual void Start( void ) 
    {
        MPIX_CALLWORLD( MPI_Start( (MPI_Request *) &the_real_request ));
    }
    static void Startall( int v1, Prequest v2[] ) 
    {
        MPI_Request *l2 = new MPI_Request[v1];
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                l2[i2] = v2[i2].the_real_request;
            }
        }
        MPIX_CALLWORLD( MPI_Startall( v1, l2 ));
        { 
            int i2; 
            for (i2=0;i2<v1;i2++) {
                v2[i2].the_real_request = l2[i2];
            }
            delete[] l2;
        }
    }
};

class Comm  {
    friend class Cartcomm;
    friend class Intercomm;
    friend class Intracomm;
    friend class Graphcomm;
    friend class Nullcomm;
    friend class Datatype;
    friend class Win;
    friend class File;

  protected:
    MPI_Comm the_real_comm;

  public:
    // new/delete

    inline Comm(MPI_Comm obj) : the_real_comm(obj) {}
    inline Comm(void) : the_real_comm(MPI_COMM_NULL) {}

    virtual ~Comm() {}
    // copy/assignment

    Comm(const Comm &obj) : the_real_comm(obj.the_real_comm){}

    Comm& operator=(const Comm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // logical
    bool operator== (const Comm &obj) {
      return (the_real_comm == obj.the_real_comm); }
    bool operator!= (const Comm &obj) {
      return (the_real_comm != obj.the_real_comm); }
    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Comm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }
    virtual Group Get_group( void ) const
    {
        Group v2;
        MPIX_CALLREF( this, MPI_Comm_group( (MPI_Comm) the_real_comm, &(v2.the_real_group) ));
        return v2;
    }
    virtual int Get_rank( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Comm_rank( (MPI_Comm) the_real_comm, &v2 ));
        return v2;
    }
    virtual Prequest Bsend_init( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Prequest v7;
        MPIX_CALLREF( this, MPI_Bsend_init( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual Prequest Ssend_init( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Prequest v7;
        MPIX_CALLREF( this, MPI_Ssend_init( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual bool Is_inter( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Comm_test_inter( (MPI_Comm) the_real_comm, &v2 ));
        return v2!= 0;
    }
    virtual Prequest Rsend_init( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Prequest v7;
        MPIX_CALLREF( this, MPI_Rsend_init( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual Request Ibsend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Request v7;
        MPIX_CALLREF( this, MPI_Ibsend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual void Abort( int v2 ) const
    {
        MPIX_CALLREF( this, MPI_Abort( (MPI_Comm) the_real_comm, v2 ));
    }
    virtual void Free( void ) 
    {
        MPIX_CALLREF( this, MPI_Comm_free( (MPI_Comm *) &the_real_comm ));
    }
    virtual Prequest Send_init( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Prequest v7;
        MPIX_CALLREF( this, MPI_Send_init( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual void Recv( void * v1, int v2, const Datatype &v3, int v4, int v5, Status & v7 ) const
    {
        MPIX_CALLREF( this, MPI_Recv( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, (MPI_Status *)&(v7.the_real_status ) ));
    }
    virtual void Recv( void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        MPIX_CALLREF( this, MPI_Recv( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, MPI_STATUS_IGNORE ));
    }
    virtual void Sendrecv( const void * v1, int v2, const Datatype &v3, int v4, int v5, void * v6, int v7, const Datatype &v8, int v9, int v10, Status & v12 ) const
    {
        MPIX_CALLREF( this, MPI_Sendrecv( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, v7, (MPI_Datatype)(v8.the_real_datatype), v9, v10, (MPI_Comm) the_real_comm, (MPI_Status *)&(v12.the_real_status ) ));
    }
    virtual void Sendrecv( const void * v1, int v2, const Datatype &v3, int v4, int v5, void * v6, int v7, const Datatype &v8, int v9, int v10 ) const
    {
        MPIX_CALLREF( this, MPI_Sendrecv( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, v7, (MPI_Datatype)(v8.the_real_datatype), v9, v10, (MPI_Comm) the_real_comm, MPI_STATUS_IGNORE ));
    }
    virtual void Sendrecv_replace( void * v1, int v2, const Datatype &v3, int v4, int v5, int v6, int v7, Status & v9 ) const
    {
        MPIX_CALLREF( this, MPI_Sendrecv_replace( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, v7, (MPI_Comm) the_real_comm, (MPI_Status *)&(v9.the_real_status ) ));
    }
    virtual void Sendrecv_replace( void * v1, int v2, const Datatype &v3, int v4, int v5, int v6, int v7 ) const
    {
        MPIX_CALLREF( this, MPI_Sendrecv_replace( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, v7, (MPI_Comm) the_real_comm, MPI_STATUS_IGNORE ));
    }
    virtual int Get_topology( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Topo_test( (MPI_Comm) the_real_comm, &v2 ));
        return v2;
    }
    virtual Request Isend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Request v7;
        MPIX_CALLREF( this, MPI_Isend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual void Probe( int v1, int v2, Status & v4 ) const
    {
        MPIX_CALLREF( this, MPI_Probe( v1, v2, (MPI_Comm) the_real_comm, (MPI_Status *)&(v4.the_real_status ) ));
    }
    virtual void Probe( int v1, int v2 ) const
    {
        MPIX_CALLREF( this, MPI_Probe( v1, v2, (MPI_Comm) the_real_comm, MPI_STATUS_IGNORE ));
    }
    static int Compare( const Comm &v1, const Comm &v2 ) 
    {
        int v3;
        MPIX_CALLWORLD( MPI_Comm_compare( (MPI_Comm)(v1.the_real_comm), (MPI_Comm)(v2.the_real_comm), &v3 ));
        return v3;
    }
    virtual int Get_size( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Comm_size( (MPI_Comm) the_real_comm, &v2 ));
        return v2;
    }
    virtual Request Issend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Request v7;
        MPIX_CALLREF( this, MPI_Issend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual void Set_errhandler( const Errhandler &v2 ) 
    {
        MPIX_CALLREF( this, MPI_Comm_set_errhandler( (MPI_Comm) the_real_comm, (MPI_Errhandler)(v2.the_real_errhandler) ));
    }
    virtual void Send( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        MPIX_CALLREF( this, MPI_Send( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm ));
    }
    virtual Request Irsend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Request v7;
        MPIX_CALLREF( this, MPI_Irsend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual void Ssend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        MPIX_CALLREF( this, MPI_Ssend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm ));
    }
    virtual Prequest Recv_init( void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Prequest v7;
        MPIX_CALLREF( this, MPI_Recv_init( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual bool Iprobe( int v1, int v2, Status & v5 ) const
    {
        int v4;
        MPIX_CALLREF( this, MPI_Iprobe( v1, v2, (MPI_Comm) the_real_comm, &v4, (MPI_Status *)&(v5.the_real_status ) ));
        return v4!= 0;
    }
    virtual bool Iprobe( int v1, int v2 ) const
    {
        int v4;
        MPIX_CALLREF( this, MPI_Iprobe( v1, v2, (MPI_Comm) the_real_comm, &v4, MPI_STATUS_IGNORE ));
        return v4!= 0;
    }
    virtual void Bsend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        MPIX_CALLREF( this, MPI_Bsend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm ));
    }
    virtual Request Irecv( void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        Request v7;
        MPIX_CALLREF( this, MPI_Irecv( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm, &(v7.the_real_request) ));
        return v7;
    }
    virtual Errhandler Get_errhandler( void ) const
    {
        Errhandler v2;
        MPIX_CALLREF( this, MPI_Comm_get_errhandler( (MPI_Comm) the_real_comm, &(v2.the_real_errhandler) ));
        return v2;
    }
    virtual void Rsend( const void * v1, int v2, const Datatype &v3, int v4, int v5 ) const
    {
        MPIX_CALLREF( this, MPI_Rsend( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Comm) the_real_comm ));
    }
    virtual void Gatherv( const void * v1, int v2, const Datatype &v3, void * v4, const int * v5, const int * v6, const Datatype &v7, int v8 ) const
    {
        MPIX_CALLREF( this, MPI_Gatherv( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, (const int *)v5, (const int *)v6, (MPI_Datatype)(v7.the_real_datatype), v8, (MPI_Comm) the_real_comm ));
    }
    virtual void Disconnect( void ) 
    {
        MPIX_CALLREF( this, MPI_Comm_disconnect( (MPI_Comm *) &the_real_comm ));
    }
    virtual void Allreduce( const void * v1, void * v2, int v3, const Datatype &v4, const Op &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Allreduce( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), (MPI_Comm) the_real_comm ));
    }
    virtual void Alltoallw( const void * v1, const int v2[], const int v3[],  const Datatype v4[], void * v5, const int v6[], const int v7[],  const Datatype v8[] ) const
    {
        MPI_Datatype *l4 = new MPI_Datatype[Get_size()];
        MPI_Datatype *l8 = new MPI_Datatype[Get_size()];
        { 
            int i4; 
            for (i4=0;i4<Get_size();i4++) {
                l4[i4] = v4[i4].the_real_datatype;
            }
        }
        { 
            int i8; 
            for (i8=0;i8<Get_size();i8++) {
                l8[i8] = v8[i8].the_real_datatype;
            }
        }
        MPIX_CALLREF( this, MPI_Alltoallw( (const void *)v1, (const int  *)v2, (const int  *)v3, l4, v5, (const int  *)v6, (const int  *)v7, l8, (MPI_Comm) the_real_comm ));
                    delete[] l4;
                    delete[] l8;
    }
    static Intercomm Join( const int v1 ) ;
    virtual void Alltoall( const void * v1, int v2, const Datatype &v3, void * v4, int v5, const Datatype &v6 ) const
    {
        MPIX_CALLREF( this, MPI_Alltoall( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Datatype)(v6.the_real_datatype), (MPI_Comm) the_real_comm ));
    }
    virtual bool Get_attr( int v2, void * v3 ) const
    {
        int v4;
        MPIX_CALLREF( this, MPI_Comm_get_attr( (MPI_Comm) the_real_comm, v2, v3, &v4 ));
        return v4!= 0;
    }
    virtual void Barrier( void ) const
    {
        MPIX_CALLREF( this, MPI_Barrier( (MPI_Comm) the_real_comm ));
    }
    virtual void Bcast( void * v1, int v2, const Datatype &v3, int v4 ) const
    {
        MPIX_CALLREF( this, MPI_Bcast( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, (MPI_Comm) the_real_comm ));
    }
    virtual void Set_attr( int v2, const void * v3 ) 
    {
        MPIX_CALLREF( this, MPI_Comm_set_attr( (MPI_Comm) the_real_comm, v2, (void *)v3 ));
    }
    virtual void Set_name( const char * v2 ) 
    {
        MPIX_CALLREF( this, MPI_Comm_set_name( (MPI_Comm) the_real_comm, (const char *)v2 ));
    }
    static Intercomm Get_parent( void ) ;
    virtual void Alltoallv( const void * v1, const int * v2, const int * v3, const Datatype &v4, void * v5, const int * v6, const int * v7, const Datatype &v8 ) const
    {
        MPIX_CALLREF( this, MPI_Alltoallv( (const void *)v1, (const int *)v2, (const int *)v3, (MPI_Datatype)(v4.the_real_datatype), v5, (const int *)v6, (const int *)v7, (MPI_Datatype)(v8.the_real_datatype), (MPI_Comm) the_real_comm ));
    }
    virtual void Reduce_scatter( const void * v1, void * v2, const int v3[], const Datatype &v4, const Op &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Reduce_scatter( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), (MPI_Comm) the_real_comm ));
    }
    virtual void Scatter( const void * v1, int v2, const Datatype &v3, void * v4, int v5, const Datatype &v6, int v7 ) const
    {
        MPIX_CALLREF( this, MPI_Scatter( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Datatype)(v6.the_real_datatype), v7, (MPI_Comm) the_real_comm ));
    }
    virtual void Gather( const void * v1, int v2, const Datatype &v3, void * v4, int v5, const Datatype &v6, int v7 ) const
    {
        MPIX_CALLREF( this, MPI_Gather( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Datatype)(v6.the_real_datatype), v7, (MPI_Comm) the_real_comm ));
    }
    static void Free_keyval( int &v1 ) 
    {
        MPIX_CALLWORLD( MPI_Comm_free_keyval( &v1 ));
    }
    virtual void Reduce( const void * v1, void * v2, int v3, const Datatype &v4, const Op &v5, int v6 ) const
    {
        MPIX_CALLREF( this, MPI_Reduce( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), v6, (MPI_Comm) the_real_comm ));
    }
    virtual void Allgather( const void * v1, int v2, const Datatype &v3, void * v4, int v5, const Datatype &v6 ) const
    {
        MPIX_CALLREF( this, MPI_Allgather( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, (MPI_Datatype)(v6.the_real_datatype), (MPI_Comm) the_real_comm ));
    }
    virtual void Delete_attr( int v2 ) 
    {
        MPIX_CALLREF( this, MPI_Comm_delete_attr( (MPI_Comm) the_real_comm, v2 ));
    }
    virtual void Scatterv( const void * v1, const int * v2, const int * v3, const Datatype &v4, void * v5, int v6, const Datatype &v7, int v8 ) const
    {
        MPIX_CALLREF( this, MPI_Scatterv( (const void *)v1, (const int *)v2, (const int *)v3, (MPI_Datatype)(v4.the_real_datatype), v5, v6, (MPI_Datatype)(v7.the_real_datatype), v8, (MPI_Comm) the_real_comm ));
    }
    virtual void Get_name( char * v2, int &v3 ) const
    {
        MPIX_CALLREF( this, MPI_Comm_get_name( (MPI_Comm) the_real_comm, v2, &v3 ));
    }
    virtual void Allgatherv( const void * v1, int v2, const Datatype &v3, void * v4, const int * v5, const int * v6, const Datatype &v7 ) const
    {
        MPIX_CALLREF( this, MPI_Allgatherv( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, (const int *)v5, (const int *)v6, (MPI_Datatype)(v7.the_real_datatype), (MPI_Comm) the_real_comm ));
    }
    virtual Comm &Clone(void) const = 0;
    typedef int Copy_attr_function(const Comm& oldcomm, int comm_keyval, void* extra_state, void* attribute_val_in, void* attribute_val_out, bool& flag); 
    typedef int Delete_attr_function(Comm& comm, int comm_keyval, void* attribute_val, void* extra_state); 
    typedef void Errhandler_function(Comm &, int *, ... );
    typedef Errhandler_function Errhandler_fn;

    static int Create_keyval( Copy_attr_function *, Delete_attr_function *,
                              void * );
          
    static int NULL_COPY_FN( const Comm &oldcomm MPIR_ARGUNUSED, 
           int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
           void *attr_in MPIR_ARGUNUSED, void *attr_out MPIR_ARGUNUSED, 
	   bool &flag ) { flag = 0; return 0;}
    static int NULL_DELETE_FN( Comm &comm MPIR_ARGUNUSED, 
	   int keyval MPIR_ARGUNUSED, void * attr MPIR_ARGUNUSED, 
	   void *ex MPIR_ARGUNUSED ) { return 0; }
    static int DUP_FN( const Comm &oldcomm MPIR_ARGUNUSED, 
           int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
           void *attr_in, void *attr_out, bool &flag ) { flag = 1; 
                    *(void **)attr_out = attr_in; return 0;} 
    static Errhandler Create_errhandler( Errhandler_function * );

    virtual void Call_errhandler( int v2 ) const;
    virtual void Reduce_scatter_block( const void * v1, void * v2, int v3, const Datatype &v4, const Op &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Reduce_scatter_block( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), (MPI_Comm) the_real_comm ));
    }
};

class Nullcomm : public Comm {

  public:
    // new/delete

    inline Nullcomm(MPI_Comm obj) : Comm(obj) {}
    inline Nullcomm(void) : Comm() {}

    virtual ~Nullcomm() {}
    // copy/assignment

    Nullcomm(const Nullcomm &obj) : Comm(obj) {}

    Nullcomm& operator=(const Nullcomm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Nullcomm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }

// If the compiler does not support variable return types, return a 
// reference to Comm.  The user must then cast this to the correct type
// (Standard-conforming C++ compilers support variable return types)
#ifdef HAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT
    virtual Comm & Clone(void) const { 
        Comm *clone = new Nullcomm(MPI_COMM_NULL); 
        return *clone; 
    };
#else
    virtual Nullcomm & Clone(void) const { 
        Nullcomm *clone = new Nullcomm();
        return *clone; 
    };
#endif
};

class Intercomm : public Comm {
    friend class Intracomm;

  public:
    // new/delete

    inline Intercomm(MPI_Comm obj) : Comm(obj) {}
    inline Intercomm(void) : Comm() {}

    virtual ~Intercomm() {}
    // copy/assignment

    Intercomm(const Intercomm &obj) : Comm(obj) {}

    Intercomm& operator=(const Intercomm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Intercomm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }
    virtual Intracomm Merge( bool v2 ) const;
    virtual Group Get_remote_group( void ) const
    {
        Group v2;
        MPIX_CALLREF( this, MPI_Comm_remote_group( (MPI_Comm) the_real_comm, &(v2.the_real_group) ));
        return v2;
    }
    virtual int Get_remote_size( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Comm_remote_size( (MPI_Comm) the_real_comm, &v2 ));
        return v2;
    }
    Intercomm Dup( void ) const
    {
        Intercomm v2;
        MPIX_CALLREF( this, MPI_Comm_dup( (MPI_Comm) the_real_comm, &(v2.the_real_comm) ));
        return v2;
    }
    virtual Intercomm Split( int v2, int v3 ) const
    {
        Intercomm v4;
        MPIX_CALLREF( this, MPI_Comm_split( (MPI_Comm) the_real_comm, v2, v3, &(v4.the_real_comm) ));
        return v4;
    }
    
// If the compiler does not support variable return types, return a 
// reference to Comm.  The user must then cast this to the correct type
// (Standard-conforming C++ compilers support variable return types)
#ifdef HAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT
    virtual Comm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Comm *clone = new Intercomm(ncomm); 
        return *clone; 
    };
#else
    virtual Intercomm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Intercomm *clone = new Intercomm(ncomm); 
        return *clone; 
    };
#endif
};

class Intracomm : public Comm {
    friend class Cartcomm;
    friend class Graphcomm;
    friend class Datatype;

  public:
    // new/delete

    inline Intracomm(MPI_Comm obj) : Comm(obj) {}
    inline Intracomm(void) : Comm() {}

    virtual ~Intracomm() {}
    // copy/assignment

    Intracomm(const Intracomm &obj) : Comm(obj) {}

    Intracomm& operator=(const Intracomm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Intracomm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }
    virtual Intercomm Create_intercomm( int v2, const Comm &v3, int v4, int v5 ) const
    {
        Intercomm v6;
        MPIX_CALLREF( this, MPI_Intercomm_create( (MPI_Comm) the_real_comm, v2, (MPI_Comm)(v3.the_real_comm), v4, v5, &(v6.the_real_comm) ));
        return v6;
    }
    virtual Intracomm Split( int v2, int v3 ) const
    {
        Intracomm v4;
        MPIX_CALLREF( this, MPI_Comm_split( (MPI_Comm) the_real_comm, v2, v3, &(v4.the_real_comm) ));
        return v4;
    }
    virtual Graphcomm Create_graph( int v2, const int v3[], const int v4[], bool v5 ) const;
    virtual Cartcomm Create_cart( int v2, const int v3[], const bool v4[], bool v5 ) const;
    virtual Intracomm Create( const Group &v2 ) const
    {
        Intracomm v3;
        MPIX_CALLREF( this, MPI_Comm_create( (MPI_Comm) the_real_comm, (MPI_Group)(v2.the_real_group), &(v3.the_real_comm) ));
        return v3;
    }
    Intracomm Dup( void ) const
    {
        Intracomm v2;
        MPIX_CALLREF( this, MPI_Comm_dup( (MPI_Comm) the_real_comm, &(v2.the_real_comm) ));
        return v2;
    }
    virtual void Scan( const void * v1, void * v2, int v3, const Datatype &v4, const Op &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Scan( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), (MPI_Comm) the_real_comm ));
    }
    virtual void Exscan( const void * v1, void * v2, int v3, const Datatype &v4, const Op &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Exscan( (const void *)v1, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Op)(v5.the_real_op), (MPI_Comm) the_real_comm ));
    }
    virtual Intercomm Accept( const char * v1, const Info &v2, int v3 ) const
    {
        Intercomm v5;
        MPIX_CALLREF( this, MPI_Comm_accept( (const char *)v1, (MPI_Info)(v2.the_real_info), v3, (MPI_Comm) the_real_comm, &(v5.the_real_comm) ));
        return v5;
    }
    virtual Intercomm Connect( const char * v1, const Info &v2, int v3 ) const
    {
        Intercomm v5;
        MPIX_CALLREF( this, MPI_Comm_connect( (const char *)v1, (MPI_Info)(v2.the_real_info), v3, (MPI_Comm) the_real_comm, &(v5.the_real_comm) ));
        return v5;
    }
    
// If the compiler does not support variable return types, return a 
// reference to Comm.  The user must then cast this to the correct type
// (Standard-conforming C++ compilers support variable return types)
#ifdef HAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT
    virtual Comm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Comm *clone = new Intracomm(ncomm); 
        return *clone; 
    };
#else
    virtual Intracomm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Intracomm *clone = new Intracomm(ncomm); 
        return *clone; 
    };
#endif

Intercomm Spawn(const char* command, const char* argv[], int maxprocs, const MPI::Info& info, int root) const {
    Intercomm ic;
    MPIX_CALLREF( this, MPI_Comm_spawn( (char *)command, 
                (char **)argv, 
                maxprocs, info.the_real_info, root, the_real_comm, 
                &(ic.the_real_comm), MPI_ERRCODES_IGNORE ) );
    return ic;
}
Intercomm Spawn(const char* command, const char* argv[], int maxprocs, const MPI::Info& info, int root, int array_of_errcodes[]) const {
    Intercomm ic;
    MPIX_CALLREF( this, MPI_Comm_spawn( (char *)command, 
                (char **)argv, 
                maxprocs, info.the_real_info, root, the_real_comm, 
                &(ic.the_real_comm), array_of_errcodes ) );
    return ic;
}
Intercomm Spawn_multiple(int count, const char* array_of_commands[], const char** array_of_argv[], const int array_of_maxprocs[], const MPI::Info array_of_info[], int root) {
    Intercomm ic;
    MPI_Info  *li = new MPI_Info [count];
    int i;
    for (i=0; i<count; i++) {
        li[i] = array_of_info[i].the_real_info;
    }
    MPIX_CALLREF( this, MPI_Comm_spawn_multiple( count, 
                   (char **)array_of_commands, 
                   (char ***)array_of_argv, (int *)array_of_maxprocs, 
                   li, root, the_real_comm, &(ic.the_real_comm), 
                   MPI_ERRCODES_IGNORE ) );
    delete [] li;
    return ic;
}
Intercomm Spawn_multiple(int count, const char* array_of_commands[], const char** array_of_argv[], const int array_of_maxprocs[], const MPI::Info array_of_info[], int root, int array_of_errcodes[]) {
    Intercomm ic;
    MPI_Info  *li = new MPI_Info [count];
    int i;
    for (i=0; i<count; i++) {
        li[i] = array_of_info[i].the_real_info;
    }
    MPIX_CALLREF( this, MPI_Comm_spawn_multiple( count, 
                   (char **)array_of_commands, 
                   (char ***)array_of_argv, (int *)array_of_maxprocs, 
                   li, root, the_real_comm, &(ic.the_real_comm), 
                   array_of_errcodes ) );
    delete [] li;
    return ic;
}

};

class Grequest : public Request {

  public:
    // new/delete

    inline Grequest(MPI_Request obj) : Request(obj) {}
    inline Grequest(void) : Request() {}

    virtual ~Grequest() {}
    // copy/assignment

    Grequest(const Grequest &obj) : Request(obj) {}

    Grequest& operator=(const Grequest &obj) {
      the_real_request = obj.the_real_request; return *this; }

    // logical
    bool operator== (const Grequest &obj) {
      return (the_real_request == obj.the_real_request); }
    bool operator!= (const Grequest &obj) {
      return (the_real_request != obj.the_real_request); }
    // C/C++ cast and assignment
    inline operator MPI_Request*() { return &the_real_request; }
    inline operator MPI_Request() const { return the_real_request; }
    Grequest& operator=(const MPI_Request& obj) {
      the_real_request = obj; return *this; }
    virtual void Complete( void ) 
    {
        MPIX_CALLWORLD( MPI_Grequest_complete( (MPI_Request) the_real_request ));
    }

    typedef int Query_function( void *, Status & );
    typedef int Free_function( void * );
    typedef int Cancel_function( void *, bool );

    Grequest Start( Query_function  *query_fn,
                    Free_function   *free_fn,
                    Cancel_function *cancel_fn,
                    void *extra_state );
};

class Win  {

  protected:
    MPI_Win the_real_win;

  public:
    // new/delete

    inline Win(MPI_Win obj) : the_real_win(obj) {}
    inline Win(void) : the_real_win(MPI_WIN_NULL) {}

    virtual ~Win() {}
    // copy/assignment

    Win(const Win &obj) : the_real_win(obj.the_real_win){}

    Win& operator=(const Win &obj) {
      the_real_win = obj.the_real_win; return *this; }

    // logical
    bool operator== (const Win &obj) {
      return (the_real_win == obj.the_real_win); }
    bool operator!= (const Win &obj) {
      return (the_real_win != obj.the_real_win); }
    // C/C++ cast and assignment
    inline operator MPI_Win*() { return &the_real_win; }
    inline operator MPI_Win() const { return the_real_win; }
    Win& operator=(const MPI_Win& obj) {
      the_real_win = obj; return *this; }
    virtual Group Get_group( void ) const
    {
        Group v2;
        MPIX_CALLREF( this, MPI_Win_get_group( (MPI_Win) the_real_win, &(v2.the_real_group) ));
        return v2;
    }
    virtual void Fence( int v1 ) const
    {
        MPIX_CALLREF( this, MPI_Win_fence( v1, (MPI_Win) the_real_win ));
    }
    virtual void Start( const Group &v1, int v2 ) const
    {
        MPIX_CALLREF( this, MPI_Win_start( (MPI_Group)(v1.the_real_group), v2, (MPI_Win) the_real_win ));
    }
    virtual void Free( void ) 
    {
        MPIX_CALLREF( this, MPI_Win_free( (MPI_Win *) &the_real_win ));
    }
    virtual void Put( const void * v1, int v2, const Datatype &v3, int v4, Aint v5, int v6, const Datatype &v7 ) const
    {
        MPIX_CALLREF( this, MPI_Put( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, (MPI_Datatype)(v7.the_real_datatype), (MPI_Win) the_real_win ));
    }
    virtual void Wait( void ) const
    {
        MPIX_CALLREF( this, MPI_Win_wait( (MPI_Win) the_real_win ));
    }
    virtual bool Test( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Win_test( (MPI_Win) the_real_win, &v2 ));
        return v2!= 0;
    }
    virtual void Get( void * v1, int v2, const Datatype &v3, int v4, Aint v5, int v6, const Datatype &v7 ) const
    {
        MPIX_CALLREF( this, MPI_Get( v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, (MPI_Datatype)(v7.the_real_datatype), (MPI_Win) the_real_win ));
    }
    virtual bool Get_attr( int v2, void * v3 ) const
    {
        int v4;
        MPIX_CALLREF( this, MPI_Win_get_attr( (MPI_Win) the_real_win, v2, v3, &v4 ));
        return v4!= 0;
    }
    virtual void Set_attr( int v2, const void * v3 ) 
    {
        MPIX_CALLREF( this, MPI_Win_set_attr( (MPI_Win) the_real_win, v2, (void *)v3 ));
    }
    virtual void Complete( void ) const
    {
        MPIX_CALLREF( this, MPI_Win_complete( (MPI_Win) the_real_win ));
    }
    virtual void Set_errhandler( const Errhandler &v2 ) 
    {
        MPIX_CALLREF( this, MPI_Win_set_errhandler( (MPI_Win) the_real_win, (MPI_Errhandler)(v2.the_real_errhandler) ));
    }
    virtual void Set_name( const char * v2 ) 
    {
        MPIX_CALLREF( this, MPI_Win_set_name( (MPI_Win) the_real_win, (const char *)v2 ));
    }
    virtual void Accumulate( const void * v1, int v2, const Datatype &v3, int v4, Aint v5, int v6, const Datatype &v7, const Op &v8 ) const
    {
        MPIX_CALLREF( this, MPI_Accumulate( (const void *)v1, v2, (MPI_Datatype)(v3.the_real_datatype), v4, v5, v6, (MPI_Datatype)(v7.the_real_datatype), (MPI_Op)(v8.the_real_op), (MPI_Win) the_real_win ));
    }
    static Win Create( const void * v1, Aint v2, int v3, const Info &v4, const Intracomm &v5 ) 
    {
        Win v6;
        MPIX_CALLOBJ( v5, MPI_Win_create( (void *)v1, v2, v3, (MPI_Info)(v4.the_real_info), (MPI_Comm)(v5.the_real_comm), &(v6.the_real_win) ));
        return v6;
    }
    static void Free_keyval( int &v1 ) 
    {
        MPIX_CALLWORLD( MPI_Win_free_keyval( &v1 ));
    }
    virtual void Post( const Group &v1, int v2 ) const
    {
        MPIX_CALLREF( this, MPI_Win_post( (MPI_Group)(v1.the_real_group), v2, (MPI_Win) the_real_win ));
    }
    virtual void Unlock( int v1 ) const
    {
        MPIX_CALLREF( this, MPI_Win_unlock( v1, (MPI_Win) the_real_win ));
    }
    virtual void Delete_attr( int v2 ) 
    {
        MPIX_CALLREF( this, MPI_Win_delete_attr( (MPI_Win) the_real_win, v2 ));
    }
    virtual void Lock( int v1, int v2, int v3 ) const
    {
        MPIX_CALLREF( this, MPI_Win_lock( v1, v2, v3, (MPI_Win) the_real_win ));
    }
    virtual Errhandler Get_errhandler( void ) const
    {
        Errhandler v2;
        MPIX_CALLREF( this, MPI_Win_get_errhandler( (MPI_Win) the_real_win, &(v2.the_real_errhandler) ));
        return v2;
    }
    virtual void Get_name( char * v2, int &v3 ) const
    {
        MPIX_CALLREF( this, MPI_Win_get_name( (MPI_Win) the_real_win, v2, &v3 ));
    }
    typedef void Errhandler_function(Win &, int *, ... );
    typedef Errhandler_function Errhandler_fn;

    static Errhandler Create_errhandler( Errhandler_function * );

    typedef int Copy_attr_function(const Win& oldwin, int win_keyval, void* extra_state, void* attribute_val_in, void* attribute_val_out, bool& flag); 
    typedef int Delete_attr_function(Win& win, int win_keyval, void* attribute_val, void* extra_state); 

    static int Create_keyval( Copy_attr_function *, Delete_attr_function *,
                              void * );
    // These functions are *not* part of MPI-2 but are provided
    // because they should have been included
    static int NULL_COPY_FN( const Win &oldwin MPIR_ARGUNUSED, 
        int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
        void *attr_in MPIR_ARGUNUSED, void *attr_out MPIR_ARGUNUSED, 
        bool &flag ) { flag = 1; return 0;}
    static int NULL_DELETE_FN( Win &win MPIR_ARGUNUSED, 
        int keyval MPIR_ARGUNUSED, void * attr MPIR_ARGUNUSED, 
        void *ex MPIR_ARGUNUSED ) { return 0; }
    static int DUP_FN( const Win &oldwin MPIR_ARGUNUSED, 
	int keyval MPIR_ARGUNUSED, void *ex MPIR_ARGUNUSED,
        void *attr_in, void *attr_out, bool &flag ) { flag = 1; 
            *(void **)attr_out = attr_in; return 0;} 

    virtual void Call_errhandler( int v2 ) const;
};
#ifndef MPI_FILE_NULL
#define MPI_FILE_NULL 0
typedef int MPI_File;
#endif

class File  {

  protected:
    MPI_File the_real_file;

  public:
    // new/delete

    inline File(MPI_File obj) : the_real_file(obj) {}
    inline File(void) : the_real_file(MPI_FILE_NULL) {}

    virtual ~File() {}
    // copy/assignment

    File(const File &obj) : the_real_file(obj.the_real_file){}

    File& operator=(const File &obj) {
      the_real_file = obj.the_real_file; return *this; }

    // logical
    bool operator== (const File &obj) {
      return (the_real_file == obj.the_real_file); }
    bool operator!= (const File &obj) {
      return (the_real_file != obj.the_real_file); }
    // C/C++ cast and assignment
    inline operator MPI_File*() { return &the_real_file; }
    inline operator MPI_File() const { return the_real_file; }
    File& operator=(const MPI_File& obj) {
      the_real_file = obj; return *this; }
#ifdef MPI_MODE_RDONLY
    virtual Aint Get_type_extent( const Datatype &v2 ) const
    {
        MPI_Aint v3;
        MPIX_CALLREF( this, MPI_File_get_type_extent( (MPI_File) the_real_file, (MPI_Datatype)(v2.the_real_datatype), &v3 ));
        return v3;
    }
    virtual void Read_ordered_end( void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_ordered_end( (MPI_File) the_real_file, v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Read_ordered_end( void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_ordered_end( (MPI_File) the_real_file, v2, MPI_STATUS_IGNORE ));
    }
    virtual void Seek_shared( Offset v2, int v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_seek_shared( (MPI_File) the_real_file, v2, v3 ));
    }
    virtual void Read_ordered( void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_ordered( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Read_ordered( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_ordered( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual Request Iread_shared( void * v2, int v3, const Datatype &v4 ) 
    {
        Request v5;
        MPIX_CALLREF( this, MPI_File_iread_shared( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPIO_Request *)&(v5.the_real_request) ));
        return v5;
    }
    virtual Info Get_info( void ) const
    {
        Info v2;
        MPIX_CALLREF( this, MPI_File_get_info( (MPI_File) the_real_file, &(v2.the_real_info) ));
        return v2;
    }
    virtual void Write_ordered_begin( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_ordered_begin( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype) ));
    }
    virtual void Set_info( const Info &v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_set_info( (MPI_File) the_real_file, (MPI_Info)(v2.the_real_info) ));
    }
    virtual void Write_ordered( const void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_ordered( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Write_ordered( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_ordered( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Sync( void ) 
    {
        MPIX_CALLREF( this, MPI_File_sync( (MPI_File) the_real_file ));
    }
    virtual void Read( void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Read( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Write_all( const void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_all( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Write_all( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_all( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual Offset Get_size( void ) const
    {
        MPI_Offset v2;
        MPIX_CALLREF( this, MPI_File_get_size( (MPI_File) the_real_file, &v2 ));
        return v2;
    }
    virtual void Write_all_end( const void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_all_end( (MPI_File) the_real_file, (const void *)v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Write_all_end( const void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_all_end( (MPI_File) the_real_file, (const void *)v2, MPI_STATUS_IGNORE ));
    }
    static void Delete( const char * v1, const Info &v2 ) 
    {
        MPIX_CALLOBJ( FILE_NULL, MPI_File_delete( (const char *)v1, (MPI_Info)(v2.the_real_info) ));
    }
    virtual void Read_ordered_begin( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_ordered_begin( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype) ));
    }
    virtual Request Iread_at( Offset v2, void * v3, int v4, const Datatype &v5 ) 
    {
        Request v6;
        MPIX_CALLREF( this, MPI_File_iread_at( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPIO_Request *)&(v6.the_real_request) ));
        return v6;
    }
    virtual void Write_at_all_end( const void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at_all_end( (MPI_File) the_real_file, (const void *)v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Write_at_all_end( const void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at_all_end( (MPI_File) the_real_file, (const void *)v2, MPI_STATUS_IGNORE ));
    }
    virtual Offset Get_position_shared( void ) const
    {
        MPI_Offset v2;
        MPIX_CALLREF( this, MPI_File_get_position_shared( (MPI_File) the_real_file, &v2 ));
        return v2;
    }
    virtual void Write_shared( const void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_shared( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Write_shared( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_shared( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual Request Iwrite_at( Offset v2, const void * v3, int v4, const Datatype &v5 ) 
    {
        Request v6;
        MPIX_CALLREF( this, MPI_File_iwrite_at( (MPI_File) the_real_file, (MPI_Offset)v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPIO_Request *)&(v6.the_real_request) ));
        return v6;
    }
    virtual void Get_view( Offset & v2, Datatype &v3, Datatype &v4, char * v5 ) const
    {
        MPIX_CALLREF( this, MPI_File_get_view( (MPI_File) the_real_file, &v2, (MPI_Datatype *)&(v3.the_real_datatype), (MPI_Datatype *)&(v4.the_real_datatype), v5 ));
    }
    virtual void Write_all_begin( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_all_begin( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype) ));
    }
    virtual void Read_all_end( void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_all_end( (MPI_File) the_real_file, v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Read_all_end( void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_all_end( (MPI_File) the_real_file, v2, MPI_STATUS_IGNORE ));
    }
    virtual Offset Get_byte_offset( const Offset v2 ) const
    {
        MPI_Offset v3;
        MPIX_CALLREF( this, MPI_File_get_byte_offset( (MPI_File) the_real_file, (MPI_Offset)v2, &v3 ));
        return v3;
    }
    virtual Request Iread( void * v2, int v3, const Datatype &v4 ) 
    {
        Request v5;
        MPIX_CALLREF( this, MPI_File_iread( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPIO_Request *)&(v5.the_real_request) ));
        return v5;
    }
    virtual void Read_at_all_end( void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at_all_end( (MPI_File) the_real_file, v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Read_at_all_end( void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at_all_end( (MPI_File) the_real_file, v2, MPI_STATUS_IGNORE ));
    }
    virtual void Write_at( Offset v2, const void * v3, int v4, const Datatype &v5, Status & v6 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at( (MPI_File) the_real_file, v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPI_Status *)&(v6.the_real_status ) ));
    }
    virtual void Write_at( Offset v2, const void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at( (MPI_File) the_real_file, v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Write_at_all_begin( Offset v2, const void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at_all_begin( (MPI_File) the_real_file, v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype) ));
    }
    virtual Errhandler Get_errhandler( void ) const
    {
        Errhandler v2;
        MPIX_CALLREF( this, MPI_File_get_errhandler( (MPI_File) the_real_file, &(v2.the_real_errhandler) ));
        return v2;
    }
    virtual int Get_amode( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_File_get_amode( (MPI_File) the_real_file, &v2 ));
        return v2;
    }
    virtual void Set_atomicity( bool v2 ) 
    {
        int l2;
         l2 = (v2 == true) ? 1 : 0;
        MPIX_CALLREF( this, MPI_File_set_atomicity( (MPI_File) the_real_file, l2 ));
    }
    virtual Group Get_group( void ) const
    {
        Group v2;
        MPIX_CALLREF( this, MPI_File_get_group( (MPI_File) the_real_file, &(v2.the_real_group) ));
        return v2;
    }
    virtual Offset Get_position( void ) const
    {
        MPI_Offset v2;
        MPIX_CALLREF( this, MPI_File_get_position( (MPI_File) the_real_file, &v2 ));
        return v2;
    }
    static File Open( const Intracomm &v1, const char * v2, int v3, const Info &v4 ) 
    {
        File v5;
        MPIX_CALLOBJ( FILE_NULL, MPI_File_open( (MPI_Comm)(v1.the_real_comm), (const char *)v2, v3, (MPI_Info)(v4.the_real_info), &(v5.the_real_file) ));
        return v5;
    }
    virtual void Seek( Offset v2, int v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_seek( (MPI_File) the_real_file, v2, v3 ));
    }
    virtual void Read_all_begin( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_all_begin( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype) ));
    }
    virtual void Read_at_all_begin( Offset v2, void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at_all_begin( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype) ));
    }
    virtual void Read_all( void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_all( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Read_all( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_all( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Preallocate( Offset v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_preallocate( (MPI_File) the_real_file, v2 ));
    }
    virtual void Read_at_all( Offset v2, void * v3, int v4, const Datatype &v5, Status & v6 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at_all( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPI_Status *)&(v6.the_real_status ) ));
    }
    virtual void Read_at_all( Offset v2, void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at_all( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Read_shared( void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_shared( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Read_shared( void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_shared( (MPI_File) the_real_file, v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual Request Iwrite( const void * v2, int v3, const Datatype &v4 ) 
    {
        Request v5;
        MPIX_CALLREF( this, MPI_File_iwrite( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPIO_Request *)&(v5.the_real_request) ));
        return v5;
    }
    virtual Request Iwrite_shared( const void * v2, int v3, const Datatype &v4 ) 
    {
        Request v5;
        MPIX_CALLREF( this, MPI_File_iwrite_shared( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPIO_Request *)&(v5.the_real_request) ));
        return v5;
    }
    virtual void Set_errhandler( const Errhandler &v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_set_errhandler( (MPI_File) the_real_file, (MPI_Errhandler)(v2.the_real_errhandler) ));
    }
    virtual void Write_at_all( Offset v2, const void * v3, int v4, const Datatype &v5, Status & v6 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at_all( (MPI_File) the_real_file, v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPI_Status *)&(v6.the_real_status ) ));
    }
    virtual void Write_at_all( Offset v2, const void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_at_all( (MPI_File) the_real_file, v2, (const void *)v3, v4, (MPI_Datatype)(v5.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Set_size( Offset v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_set_size( (MPI_File) the_real_file, v2 ));
    }
    virtual void Set_view( Offset v2, const Datatype &v3, const Datatype v4, const char * v5, const Info &v6 ) 
    {
        MPIX_CALLREF( this, MPI_File_set_view( (MPI_File) the_real_file, v2, (MPI_Datatype)(v3.the_real_datatype), (MPI_Datatype)v4, (const char *)v5, (MPI_Info)(v6.the_real_info) ));
    }
    virtual void Read_at( Offset v2, void * v3, int v4, const Datatype &v5, Status & v6 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype), (MPI_Status *)&(v6.the_real_status ) ));
    }
    virtual void Read_at( Offset v2, void * v3, int v4, const Datatype &v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_read_at( (MPI_File) the_real_file, v2, v3, v4, (MPI_Datatype)(v5.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual void Close( void ) 
    {
        MPIX_CALLREF( this, MPI_File_close( (MPI_File *) &the_real_file ));
    }
    virtual void Write_ordered_end( const void * v2, Status & v3 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_ordered_end( (MPI_File) the_real_file, (const void *)v2, (MPI_Status *)&(v3.the_real_status ) ));
    }
    virtual void Write_ordered_end( const void * v2 ) 
    {
        MPIX_CALLREF( this, MPI_File_write_ordered_end( (MPI_File) the_real_file, (const void *)v2, MPI_STATUS_IGNORE ));
    }
    virtual void Write( const void * v2, int v3, const Datatype &v4, Status & v5 ) 
    {
        MPIX_CALLREF( this, MPI_File_write( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), (MPI_Status *)&(v5.the_real_status ) ));
    }
    virtual void Write( const void * v2, int v3, const Datatype &v4 ) 
    {
        MPIX_CALLREF( this, MPI_File_write( (MPI_File) the_real_file, (const void *)v2, v3, (MPI_Datatype)(v4.the_real_datatype), MPI_STATUS_IGNORE ));
    }
    virtual bool Get_atomicity( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_File_get_atomicity( (MPI_File) the_real_file, &v2 ));
        return v2!= 0;
    }
    typedef void Errhandler_function(File &, int *, ... );
    typedef Errhandler_function Errhandler_fn;

    static Errhandler Create_errhandler( Errhandler_function * );

    virtual void Call_errhandler( int v2 ) const;
#endif
};

class Graphcomm : public Intracomm {

  public:
    // new/delete

    inline Graphcomm(MPI_Comm obj) : Intracomm(obj) {}
    inline Graphcomm(void) : Intracomm() {}

    virtual ~Graphcomm() {}
    // copy/assignment

    Graphcomm(const Graphcomm &obj) : Intracomm(obj) {}

    Graphcomm& operator=(const Graphcomm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Graphcomm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }
    virtual void Get_dims( int * v2, int * v3 ) const
    {
        MPIX_CALLREF( this, MPI_Graphdims_get( (MPI_Comm) the_real_comm, v2, v3 ));
    }
    virtual void Get_topo( int v2, int v3, int v4[], int v5[] ) const
    {
        MPIX_CALLREF( this, MPI_Graph_get( (MPI_Comm) the_real_comm, v2, v3, v4, v5 ));
    }
    virtual int Map( int v2, const int v3[], const int v4[] ) const
    {
        int v5;
        MPIX_CALLREF( this, MPI_Graph_map( (MPI_Comm) the_real_comm, v2, (const int  *)v3, (const int  *)v4, &v5 ));
        return v5;
    }
    virtual void Get_neighbors( int v2, int v3, int v4[] ) const
    {
        MPIX_CALLREF( this, MPI_Graph_neighbors( (MPI_Comm) the_real_comm, v2, v3, v4 ));
    }
    virtual int Get_neighbors_count( int v2 ) const
    {
        int v3;
        MPIX_CALLREF( this, MPI_Graph_neighbors_count( (MPI_Comm) the_real_comm, v2, &v3 ));
        return v3;
    }
    Graphcomm Dup( void ) const
    {
        Graphcomm v2;
        MPIX_CALLREF( this, MPI_Comm_dup( (MPI_Comm) the_real_comm, &(v2.the_real_comm) ));
        return v2;
    }
    
// If the compiler does not support variable return types, return a 
// reference to Comm.  The user must then cast this to the correct type
// (Standard-conforming C++ compilers support variable return types)
#ifdef HAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT
    virtual Comm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Comm *clone = new Graphcomm(ncomm); 
        return *clone; 
    };
#else
    virtual Graphcomm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Graphcomm *clone = new Graphcomm(ncomm); 
        return *clone; 
    };
#endif
};

class Cartcomm : public Intracomm {

  public:
    // new/delete

    inline Cartcomm(MPI_Comm obj) : Intracomm(obj) {}
    inline Cartcomm(void) : Intracomm() {}

    virtual ~Cartcomm() {}
    // copy/assignment

    Cartcomm(const Cartcomm &obj) : Intracomm(obj) {}

    Cartcomm& operator=(const Cartcomm &obj) {
      the_real_comm = obj.the_real_comm; return *this; }

    // C/C++ cast and assignment
    inline operator MPI_Comm*() { return &the_real_comm; }
    inline operator MPI_Comm() const { return the_real_comm; }
    Cartcomm& operator=(const MPI_Comm& obj) {
      the_real_comm = obj; return *this; }
    virtual void Get_coords( int v2, int v3, int v4[] ) const
    {
        MPIX_CALLREF( this, MPI_Cart_coords( (MPI_Comm) the_real_comm, v2, v3, v4 ));
    }
    virtual int Get_cart_rank( const int v2[] ) const
    {
        int v3;
        MPIX_CALLREF( this, MPI_Cart_rank( (MPI_Comm) the_real_comm, (const int  *)v2, &v3 ));
        return v3;
    }
    virtual int Get_dim( void ) const
    {
        int v2;
        MPIX_CALLREF( this, MPI_Cartdim_get( (MPI_Comm) the_real_comm, &v2 ));
        return v2;
    }
    Cartcomm Dup( void ) const
    {
        Cartcomm v2;
        MPIX_CALLREF( this, MPI_Comm_dup( (MPI_Comm) the_real_comm, &(v2.the_real_comm) ));
        return v2;
    }
    virtual void Get_topo( int v2, int v3[], bool v4[], int v5[] ) const
    {
        int *l4 = new int[v2];
        MPIX_CALLREF( this, MPI_Cart_get( (MPI_Comm) the_real_comm, v2, v3, l4, v5 ));
        { 
            int i4; 
            for (i4=0;i4<v2;i4++) {
		// Unfortunately, at least one C++ compiler (Microsoft's)
		// generates warning messages when the type size changes
		// even when an explicit cast is used.  To avoid these messages, we 
		// cause the generated code to explicitly compute a
		// boolean value
                v4[i4] = l4[i4] != 0;
            }
            delete[] l4;
        }
    }
    virtual int Map( int v2, const int v3[], const bool v4[] ) const
    {
        int v5;
        int *l4 = new int[v2];
        { 
            int i4; 
            for (i4=0;i4<v2;i4++) {
                l4[i4] = v4[i4] == true ? 1 : 0;
            }
        }
        MPIX_CALLREF( this, MPI_Cart_map( (MPI_Comm) the_real_comm, v2, (const int  *)v3, l4, &v5 ));

            delete[] l4;
        return v5;
    }
    virtual Cartcomm Sub( const bool v2[] ) const
    {
        Cartcomm v3;
        int *l2 = new int[10];
        { 
            int i2; 
            for (i2=0;i2<10;i2++) {
                l2[i2] = v2[i2] == true ? 1 : 0;
            }
        }
        MPIX_CALLREF( this, MPI_Cart_sub( (MPI_Comm) the_real_comm, l2, &(v3.the_real_comm) ));

            delete[] l2;
        return v3;
    }
    virtual void Shift( int v2, int v3, int &v4, int &v5 ) const
    {
        MPIX_CALLREF( this, MPI_Cart_shift( (MPI_Comm) the_real_comm, v2, v3, &v4, &v5 ));
    }
    
// If the compiler does not support variable return types, return a 
// reference to Comm.  The user must then cast this to the correct type
// (Standard-conforming C++ compilers support variable return types)
#ifdef HAVE_NO_VARIABLE_RETURN_TYPE_SUPPORT
    virtual Comm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Comm *clone = new Cartcomm(ncomm); 
        return *clone; 
    };
#else
    virtual Cartcomm & Clone(void) const { 
        MPI_Comm ncomm;
        MPI_Comm_dup( (MPI_Comm)the_real_comm, &ncomm); 
        Cartcomm *clone = new Cartcomm(ncomm); 
        return *clone; 
    };
#endif
};
extern     int Add_error_class( void ) ;
extern     void* Alloc_mem( Aint v1, const Info &v2 ) ;
extern     void Lookup_name( const char * v1, const Info &v2, char * v3 ) ;
extern     void Publish_name( const char * v1, const Info &v2, const char * v3 ) ;
extern     void Unpublish_name( const char * v1, const Info &v2, const char * v3 ) ;
extern     Aint Get_address( const void * v1 ) ;
extern     void Add_error_string( int v1, const char * v2 ) ;
extern     int Query_thread( void ) ;
extern     void Close_port( const char * v1 ) ;
extern     int Add_error_code( int v1 ) ;
extern     void Free_mem( void * v1 ) ;
extern     void Open_port( const Info &v1, char * v2 ) ;
extern     bool Is_finalized( void ) ;
extern     bool Is_thread_main( void ) ;

#ifdef MPI_MODE_RDONLY
typedef int Datarep_extent_function( const Datatype&, Aint&, void *);
typedef int Datarep_conversion_function( void *, Datatype &, int, void *,
                Offset, void * );
#endif


extern Datatype CHAR;
extern Datatype UNSIGNED_CHAR;
extern Datatype BYTE;
extern Datatype SHORT;
extern Datatype UNSIGNED_SHORT;
extern Datatype INT;
extern Datatype UNSIGNED;
extern Datatype LONG;
extern Datatype UNSIGNED_LONG;
extern Datatype FLOAT;
extern Datatype DOUBLE;
extern Datatype LONG_DOUBLE;
extern Datatype LONG_LONG_INT;
extern Datatype LONG_LONG;
extern Datatype PACKED;
extern Datatype LB;
extern Datatype UB;
extern Datatype FLOAT_INT;
extern Datatype DOUBLE_INT;
extern Datatype LONG_INT;
extern Datatype SHORT_INT;
extern Datatype LONG_DOUBLE_INT;
extern Datatype REAL4;
extern Datatype REAL8;
extern Datatype REAL16;
extern Datatype COMPLEX8;
extern Datatype COMPLEX16;
extern Datatype COMPLEX32;
extern Datatype INTEGER1;
extern Datatype INTEGER2;
extern Datatype INTEGER4;
extern Datatype INTEGER8;
extern Datatype INTEGER16;
extern Datatype WCHAR;
extern Datatype SIGNED_CHAR;
extern Datatype UNSIGNED_LONG_LONG;
extern Datatype TWOINT;
extern Datatype BOOL;
#define MPIR_CXX_BOOL 0x4c000133
extern Datatype COMPLEX;
#define MPIR_CXX_COMPLEX 0x4c000834
extern Datatype DOUBLE_COMPLEX;
#define MPIR_CXX_DOUBLE_COMPLEX 0x4c001035
extern Datatype LONG_DOUBLE_COMPLEX;
#define MPIR_CXX_LONG_DOUBLE_COMPLEX 0x4c002036
extern Datatype DATATYPE_NULL;

#if 1
extern Datatype INTEGER;
extern Datatype REAL;
extern Datatype DOUBLE_PRECISION;
extern Datatype F_COMPLEX;
extern Datatype F_DOUBLE_COMPLEX;
extern Datatype LOGICAL;
extern Datatype CHARACTER;
extern Datatype TWOREAL;
extern Datatype TWODOUBLE_PRECISION;
extern Datatype TWOINTEGER;
#endif
extern const Op MAX;
extern const Op MIN;
extern const Op SUM;
extern const Op PROD;
extern const Op LAND;
extern const Op BAND;
extern const Op LOR;
extern const Op BOR;
extern const Op LXOR;
extern const Op BXOR;
extern const Op MINLOC;
extern const Op MAXLOC;
extern const Op REPLACE;
extern const Op OP_NULL;
extern Intracomm COMM_SELF;
extern const Group GROUP_EMPTY;
extern const Nullcomm COMM_NULL;
extern const Group GROUP_NULL;
extern const Request REQUEST_NULL;
extern const Errhandler ERRHANDLER_NULL;
extern const Errhandler ERRORS_RETURN;
extern const Errhandler ERRORS_ARE_FATAL;
extern const Errhandler ERRORS_THROW_EXCEPTIONS;
extern const Info INFO_NULL;
extern const Win WIN_NULL;
extern const int BSEND_OVERHEAD;
extern const int KEYVAL_INVALID;
extern const int CART;
extern const int GRAPH;
extern const int IDENT;
extern const int SIMILAR;
extern const int CONGRUENT;
extern const int UNEQUAL;
extern const int PROC_NULL;
extern const int ANY_TAG;
extern const int ANY_SOURCE;
extern const int ROOT;
extern const int TAG_UB;
extern const int IO;
extern const int HOST;
extern const int WTIME_IS_GLOBAL;
extern const int UNIVERSE_SIZE;
extern const int LASTUSEDCODE;
extern const int APPNUM;
extern const int MAX_PROCESSOR_NAME;
extern const int MAX_ERROR_STRING;
extern const int MAX_PORT_NAME;
extern const int MAX_OBJECT_NAME;
extern const int MAX_INFO_VAL;
extern const int MAX_INFO_KEY;
extern const int UNDEFINED;
extern const int LOCK_EXCLUSIVE;
extern const int LOCK_SHARED;
extern const int WIN_BASE;
extern const int WIN_DISP_UNIT;
extern const int WIN_SIZE;
extern const int SUCCESS;
extern const int ERR_BUFFER;
extern const int ERR_COUNT;
extern const int ERR_TYPE;
extern const int ERR_TAG;
extern const int ERR_COMM;
extern const int ERR_RANK;
extern const int ERR_REQUEST;
extern const int ERR_ROOT;
extern const int ERR_GROUP;
extern const int ERR_OP;
extern const int ERR_TOPOLOGY;
extern const int ERR_DIMS;
extern const int ERR_ARG;
extern const int ERR_UNKNOWN;
extern const int ERR_TRUNCATE;
extern const int ERR_OTHER;
extern const int ERR_INTERN;
extern const int ERR_PENDING;
extern const int ERR_IN_STATUS;
extern const int ERR_LASTCODE;
extern const int ERR_FILE;
extern const int ERR_ACCESS;
extern const int ERR_AMODE;
extern const int ERR_BAD_FILE;
extern const int ERR_FILE_EXISTS;
extern const int ERR_FILE_IN_USE;
extern const int ERR_NO_SPACE;
extern const int ERR_NO_SUCH_FILE;
extern const int ERR_IO;
extern const int ERR_READ_ONLY;
extern const int ERR_CONVERSION;
extern const int ERR_DUP_DATAREP;
extern const int ERR_UNSUPPORTED_DATAREP;
extern const int ERR_INFO;
extern const int ERR_INFO_KEY;
extern const int ERR_INFO_VALUE;
extern const int ERR_INFO_NOKEY;
extern const int ERR_NAME;
extern const int ERR_NO_MEM;
extern const int ERR_NOT_SAME;
extern const int ERR_PORT;
extern const int ERR_QUOTA;
extern const int ERR_SERVICE;
extern const int ERR_SPAWN;
extern const int ERR_UNSUPPORTED_OPERATION;
extern const int ERR_WIN;
extern const int ERR_BASE;
extern const int ERR_LOCKTYPE;
extern const int ERR_KEYVAL;
extern const int ERR_RMA_CONFLICT;
extern const int ERR_RMA_SYNC;
extern const int ERR_SIZE;
extern const int ERR_DISP;
extern const int ERR_ASSERT;
extern const int TYPECLASS_REAL;
extern const int TYPECLASS_INTEGER;
extern const int TYPECLASS_COMPLEX;
#if defined(MPI_SEEK_SET) && !defined(MPICH_IGNORE_CXX_SEEK) && !defined(SEEK_SET)
extern const int SEEK_SET;
extern const int SEEK_END;
extern const int SEEK_CUR;
#endif
extern const int DISTRIBUTE_BLOCK;
extern const int DISTRIBUTE_CYCLIC;
extern const int DISTRIBUTE_DFLT_DARG;
extern const int DISTRIBUTE_NONE;
extern const int ORDER_C;
extern const int ORDER_FORTRAN;
// Include these only if MPI-IO is available
#ifdef MPI_MODE_RDONLY
extern const int MAX_DATAREP_STRING;
extern const MPI_Offset DISPLACEMENT_CURRENT;
extern const int MODE_APPEND;
extern const int MODE_CREATE;
extern const int MODE_DELETE_ON_CLOSE;
extern const int MODE_EXCL;
extern const int MODE_RDONLY;
extern const int MODE_RDWR;
extern const int MODE_SEQUENTIAL;
extern const int MODE_UNIQUE_OPEN;
extern const int MODE_WRONLY;
#endif // IO
extern const int MODE_NOCHECK;
extern const int MODE_NOPRECEDE;
extern const int MODE_NOPUT;
extern const int MODE_NOSTORE;
extern const int MODE_NOSUCCEED;
extern const int COMM_TYPE_SHARED;
extern const int COMBINER_CONTIGUOUS;
extern const int COMBINER_DARRAY;
extern const int COMBINER_DUP;
extern const int COMBINER_F90_COMPLEX;
extern const int COMBINER_F90_INTEGER;
extern const int COMBINER_F90_REAL;
extern const int COMBINER_HINDEXED_INTEGER;
extern const int COMBINER_HINDEXED;
extern const int COMBINER_HVECTOR_INTEGER;
extern const int COMBINER_HVECTOR;
extern const int COMBINER_INDEXED_BLOCK;
extern const int COMBINER_INDEXED;
extern const int COMBINER_NAMED;
extern const int COMBINER_RESIZED;
extern const int COMBINER_STRUCT_INTEGER;
extern const int COMBINER_STRUCT;
extern const int COMBINER_SUBARRAY;
extern const int COMBINER_VECTOR;
extern const int COMBINER_HINDEXED_BLOCK;
extern const int THREAD_FUNNELED;
extern const int THREAD_MULTIPLE;
extern const int THREAD_SERIALIZED;
extern const int THREAD_SINGLE;
extern const char ** const ARGV_NULL;
extern const char *** const ARGVS_NULL;
extern void * const BOTTOM;
extern void * const IN_PLACE;
extern void Init(void);
extern void Init(int &, char **& );
extern int Init_thread(int);
extern int Init_thread(int &, char **&, int );
extern double Wtime(void);
extern double Wtick(void);
} // namespace MPI