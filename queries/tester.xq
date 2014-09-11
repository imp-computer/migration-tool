for $file in $fileTree//file[@suffix = "cpp"]
where $file/@size > 100
return
<li>
  {string($file/@filePath)}, size: {string($file/@size)}
</li>
