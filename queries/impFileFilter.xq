declare variable $where as xs:string := string($fileTree/@filePath);

<html>
  <head>
    <title>All cpp files in: {$where}</title>
  </head>
  <body>
    <p>
      <li>
        total audio: {
        sum($fileTree//file[
        @suffix = "m4a" or
        @suffix = "mp3" or
        @suffix = "wav" or
        @suffix = "wma"]/@size)}
      </li>
      <li>
        total video: {
        sum($fileTree//file[
        @suffix = "flv" or
        @suffix = "mov" or
        @suffix = "mp4" or
        @suffix = "avi" or
        @suffix = "mpg" or
        @suffix = "wmv"
        ]/@size)}
      </li>
      <li>
        total pictures: {
        sum($fileTree//file[
        @suffix = "bmp" or
        @suffix = "gif" or
        @suffix = "jpg" or
        @suffix = "JPG" or
        @suffix = "png" or
        @suffix = "tif"
        ]/@size)}
      </li>
      <li>
        total docs: {
        sum($fileTree//file[
        @suffix = "doc" or
        @suffix = "docx" or
        @suffix = "xls" or
        @suffix = "xlsx" or
        @suffix = "ppt"
        ]/@size)}
      </li>
    </p>
    <ul> {
        for $file in $fileTree//file[
        (: audio files :)
        @suffix = "m4a" or
        @suffix = "mp3" or
        @suffix = "wav" or
        @suffix = "wma" or

        (: video files :)
        @suffix = "flv" or
        @suffix = "mov" or
        @suffix = "mp4" or
        @suffix = "avi" or
        @suffix = "mpg" or
        @suffix = "wmv" or

        (: picture files :)
        @suffix = "bmp" or
        @suffix = "gif" or
        @suffix = "jpg" or
        @suffix = "JPG" or
        @suffix = "png" or
        @suffix = "tif" or

        (: doc files :)
        @suffix = "doc" or
        @suffix = "docx" or
        @suffix = "xls" or
        @suffix = "xlsx" or
        @suffix = "ppt"
        ]
        where $file/@size > 1000
        order by xs:integer($file/@size)
        return
        <li>
          {string($file/@filePath)}, size: {string($file/@size)}
        </li>
    } </ul>
  </body>
</html>




