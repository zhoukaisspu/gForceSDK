# ChangeLog

Version follows the rule below:

```
Version = <major>.<minor>[-Preview|RC]-<hash>-<date>
```

Token        | Description
-------------|--------------------------------------------
major        | API change
minor        | bug fixes, enhancements
Preview\|RC  | optional, incremental preview or release candidate number, incrementally increases
date         | date when this version is made
hash         | git commit hash number

**Note**:

> Both Preview release and RC release are releases preceding to a final release.
> There may be several Preview and RC releases before a final release.
>
> Preview release precedes to RC release and is less mature than RC release.
> Preview release is only for early access by selected leading customers.
> Whereas, RC release is available for the public access. Generally, after the
> first RC release, API and functionality are fixed and there will only be bug
> fixes in the subsequent RC releases and final releases. Whereas, for a
> Preview release, everything is subject to change in the subsequent release.

## v1.0-RC2-20170412
* First release for preview
