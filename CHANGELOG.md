# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## unreleased

## 1.0.0 - 2022-05-15

### Added

 - `IFACE99_VERSION_COMPATIBLE` to check for a SemVer-compatible version.
 - `IFACE99_VERSION_EQ` to check for an exact version.

## 0.8.4 - 2022-03-17

### Added

 - `DYN_LIT` that constructs interface objects out of compound literals.

## 0.8.3 - 2021-12-09

### Fixed

 - Specify `C` as the project language in `CMakeLists.txt`. Previously, CMake detected C++ and required a C++ compiler to compile the project.
 - Require CMake v3.11.4.

## 0.8.2 - 2021-12-01

### Added

 - Add the root `CMakeLists.txt` to be able to use CMake with [`FetchContent`] or [`add_subdirectory`].

[`FetchContent`]: https://cmake.org/cmake/help/latest/module/FetchContent.html
[`add_subdirectory`]: https://cmake.org/cmake/help/latest/command/add_subdirectory.html

## 0.8.1 - 2021-11-23

### Fixed

 - Programmatically require Metalang99 version compliant with 1.12.0.

## 0.8.0 - 2021-11-09

### Added

 - `VCALL`, `VCALL_OBJ`, `VCALL_SUPER`, `VCALL_SUPER_OBJ` as a convenient syntax sugar to call virtual functions.
 - `VSelf` and `VSELF(T)` to imitate typed `self` parameters.

### Changed

 - `iMethod` => `vfunc` (consistent with `VTABLE`, `vptr`) [**BC**].
 - `defaultIMethod` => `vfuncDefault` [**BC**].
 - `externImpl` => `implExtern` [**BC**].
 - `externDeclImpl` => `declImplExtern` [**BC**].
 - `self` (`VSelf`) parameters are now `restrict`.

## 0.7.0 - 2021-10-06

### Changed

 - `method` => `iMethod`, `defaultMethod` => `defaultIMethod` to avoid name clashes [**BC**].

### Fixed

- Previously, shortcuts referring to functional macros were object-like. To avoid nasty compilation errors, now they are function-like too:
   - `interface(iface)`
   - `impl(iface, implementer)`
   - `externImpl(iface, implementer)`
   - `declImpl(iface, implementer)`
   - `externDeclImpl(iface, implementer)`
   - `iMethod(ret_ty, name, ...)`
   - `defaultIMethod(ret_ty, name, ...)`
   - `DYN(implementer, iface, ...)`
   - `VTABLE(implementer, iface)`

## 0.6.0 - 2021-10-02

### Added

 - Default method implementations.

### Changed

 - Use `method` and `defaultMethod` instead of [X-Macro] for the sake of conciseness [**BC**].
 - `<iface>_INTERFACE` => `<iface>_IFACE` [**BC**].

## 0.5.0 - 2021-09-16

### Changed

 - Remove `implPrimary`, `externImplPrimary` [**BC**].

## 0.4.0 - 2021-09-14

### Changed

 - Use a variation of the [X-Macro] pattern instead of `iFn`. This change makes the implementation easier to reason about [**BC**].

[X-Macro]: https://en.wikipedia.org/wiki/X_Macro

## 0.3.0 - 2021-08-13

### Changed

 - Generate virtual tables with static linkage in `impl(Primary)` & `declImpl` [**BC**].
 - `dyn` => `DYN` to know where is a function and where is a macro.

### Added

 - Counterparts with external linkage: `externImpl(Primary)` & `externDeclImpl`.

## 0.2.0 - 2021-07-01

### Added

 - Generate `typedef struct <iface>VTable <iface>VTable;` and `typedef struct <iface> <iface>;` prior to their definitions to allow accepting `<ifaceVTable>` and `<iface>` as interface function parameters.

### Changed

 - Generate `char dummy;` only for an empty virtual table (i.e., a marker interface without superinterfaces) [**BC**].

## 0.1.0 - 2021-06-23

### Added

 - This excellent project.
