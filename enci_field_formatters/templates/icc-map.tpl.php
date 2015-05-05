<?php
/**
 * @file
 * Template file for ICC Maps.
 */
?>
<div xmlns="http://www.w3.org/1999/xhtml">
  <iframe frameborder="0" height="250" scrolling="no"
    src="http://www.icc.cat/vissir3/?lang=ca_ES&amp;zoom=6&amp;lat=<?php print $lat; ?>&amp;lon=<?php print $lon; ?>&amp;layers=B00FFFFFTFF&amp;embedded=true"
    width="233"> </iframe>
  <a href="http://www.icc.cat/vissir3/?lang=ca_ES&amp;zoom=6&amp;lat=<?php print $lat; ?>&amp;lon=<?php print $lon; ?>&amp;layers=B00FFFFFTFF&amp;embedded=true" target="_blank">
  <small>Veure mapa més gran</small></a>
</div>