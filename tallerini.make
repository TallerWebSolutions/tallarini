core = 7.x
api = 2

; Radix base theme
projects[radix][version] = 3.0-rc4
projects[radix][type] = theme
projects[radix][subdir] = contrib
projects[radix][patch][] = https://www.drupal.org/files/issues/radix-2557385-1.patch

; Touche library
libraries[touche][directory_name] = "touche"
libraries[touche][download][type] = "file"
libraries[touche][download][url] = "https://github.com/benhowdle89/touche/archive/master.zip"
