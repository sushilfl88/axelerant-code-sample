<?php
/**
 * @file
 * file for external links field formatting.
 */

/**
 * Helper function for external links XML formatting.
 */
function _enci_field_formatters_extern($items) {
  foreach ($items as $delta  =>  $item) {
    $xml  =  $item['value'];
    $avoidExt = array('pdf','net','cat','com','in','us','org');
    $attributes  =  array('org', 'xml:id', 'sample' );
    global $base_path;

    $searhTags  =  array(
      'ref' => array('searchAttr' => 'target',
            'changeAttrTo' => 'href',
            'removeAttr' => array('n', 'target'),
            'searchForString' => 'urn:ecid:',
            'replaceStringWith' => $base_path),
      'div' => array()
    );

    $specialCasesRegex = array();
    $field_xml  =  new DOMDocument();
    $field_xml->formatOutput  =  TRUE;

    if (!empty($xml)) {
      $xml = '<div>' . $xml . '</div>';
    }
    else {
      return FALSE;
    }

    $field_xml->loadXML($xml);

    //search tags in xml
    $dynamicRegex = array();

    if (!empty($searhTags)) {
      foreach ($searhTags as $keys => $value) {
        $getAttributeValue = '';
        $type = '';
        $tags_items  =  $field_xml->getElementsByTagName($keys);
        $specialTags = array_keys($specialCasesRegex);
        foreach ($tags_items as $key  =>  $tags) {
          if ($tags->tagName == 'title') {
            foreach ( $tags->childNodes as $child) {
              if ($child->nodeType  ==  XML_ELEMENT_NODE){
                  $getAttributeValue = $child->getAttribute('type');
                   if ($getAttributeValue == 'desc') {
                     $tags->removeChild($child);
                   }
              }
            }
          }

          if (in_array($keys, $specialTags)) {
            $spcial_tags_items = $field_xml->getElementsByTagName($specialCasesRegex[$keys]['checkInTag']);

            if (is_array($specialCasesRegex[$keys]['checkInAttr']) && count($spcial_tags_items) > 0) {
              foreach ($specialCasesRegex[$keys]['checkInAttr'] as $keyAttr => $keyAttrValue) {
                foreach ($spcial_tags_items as $link) {
                  $getSelectedAttributes = $link->getAttribute($keyAttr);
                  $type = $getSelectedAttributes;
                }
              }
            }
          }

          $checkInParentNode='';
          if (isset($value['checkInParentNode'])) {
            $checkInParentNode=$value['checkInParentNode'];
            $parentNode = $tags->parentNode->tagName;
          }

          if (isset($value['searchAttr'])) {
            $getAttributeValue = $tags->getAttribute($value['searchAttr']);

            if (isset($value['searchForString']) && isset($value['replaceStringWith'])) {
              $getAttributeValue = str_replace($value['searchForString'],$value['replaceStringWith'],$getAttributeValue);
            }

            if (isset($value['concateStringWith'])){
              $ext  =  pathinfo($getAttributeValue, PATHINFO_EXTENSION);
              $concateStr = $value['concateStringWith'];

              if (!in_array($ext,$avoidExt)) {
                if (is_array($concateStr)) {
                  foreach ($concateStr as $position => $concateValue) {
                    if ($position == "after") {
                      $getAttributeValue = $getAttributeValue.$concateValue;
                    }
                    else {
                      $getAttributeValue = $concateValue.$getAttributeValue;
                    }
                  }
                }
                else {
                  $getAttributeValue = $getAttributeValue.$concateValue;
                }
              }
              $getAttributeValue = str_replace($value['searchForString'],$value['replaceStringWith'],$getAttributeValue);
            }

            if (isset($value['changeAttrTo'])) {
              if (is_array($value['changeAttrTo'])) {
                foreach ($value['changeAttrTo'] as $key => $attribute) {
                  $tags->setAttribute($attribute, $tags->getAttribute($value['searchAttr'][$key]));
                }
              } else {
                $tags->setAttribute($value['changeAttrTo'],$getAttributeValue);
              }
            }
          }

          if ($tags->tagName == 'graphic') {
            foreach ($tags->attributes as $attr) {
              $type = $tags->parentNode->getAttribute('type');
              //if ($tags->hasAttribute('n')) {
                $tags->setAttribute('n', $type);
              //}
            }
          }

          if (isset($value['setNewAttr'])) {
            if (isset($type) && !empty($type)) {
              if (isset($value['setNewAttr'][$type]))
                $newAttr = $value['setNewAttr'][$type];
              else
                $newAttr = $value['setNewAttr'];
            }
            else
              $newAttr = $value['setNewAttr'];

            foreach ($newAttr as $attrType => $attrValue) {
              $chkParentsStatus=false;
               //will remove-start
              if (isset($newAttr['relationWith']) && $newAttr['relationWith']=="checkInParentNode") {
                if (in_array($parentNode, $checkInParentNode))
                  $chkParentsStatus=true;
              }
              //will remove-end
              if ($attrType!='relationWith') {
                if ($attrType == 'type') {
                  $extension = substr(strrchr($getAttributeValue, '.'), 1);
                  if ($extension !=  'ogv')
                    $attrValue = str_replace('srcExtension',$extension, $attrValue);
                  else {
                    unset($attrType);
                    unset($attrValue);
                  }
                }
                elseif ($attrType == 'href' || $attrType == 'poster') {
                  $extension = $getAttributeValue;
                  $attrValue = str_replace('searchSrc',$extension, $attrValue);
                }

                if (!is_array($attrValue)) {
                  if (isset($newAttr['relationWith'])  && $chkParentsStatus == true) {
                    $tags->setAttribute($attrType,$parentNode);
                  }
                  else if (!isset($newAttr['relationWith'])  && $chkParentsStatus == false)
                    $tags->setAttribute($attrType,$attrValue);
                }
              }
            }
          }

          if (isset($value['removeAttr'])) {
            foreach ($value['removeAttr'] as $attribute) {
              $tags->removeAttribute($attribute);
            }
          }
          if (in_array($keys, $specialTags)) {
            $spcial_tags_items = $field_xml->getElementsByTagName($specialCasesRegex[$keys]['checkInTag']);
            if (is_array($specialCasesRegex[$keys]['checkInAttr']) && count($spcial_tags_items) > 0) {
              foreach ($specialCasesRegex[$keys]['checkInAttr'] as $keyAttr => $keyAttrValue) {
                foreach ($spcial_tags_items as $link){
                  $getSelectedAttributes = $link->getAttribute($keyAttr);
                  if (isset($specialCasesRegex[$keys]['checkInAttr'][$keyAttr][$getSelectedAttributes]) && is_array($specialCasesRegex[$keys]['checkInAttr'][$keyAttr][$getSelectedAttributes])) {
                    foreach ($specialCasesRegex[$keys]['checkInAttr'][$keyAttr][$getSelectedAttributes] as $keySelectAttr => $valueSelectAttr){
                      $dynamicRegex[$keySelectAttr] = $valueSelectAttr;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    $xml = $field_xml->saveXML();
    $xml = preg_replace('/xmlns[^=]*="[^"]*"/i', '', $xml);
    $xml = str_replace('  ',' ', $xml);

    $replaceTags = $patterns  =  array(
    '/<xr (.+?) type="external">(.+?)<\/xr>/is' => '<div class="external-links">$2</div>',
    '/<xr type="external" (.+?)>(.+?)<\/xr>/is' => '<div class="external-links">$2</div>',
     '/<xr (.+?)><ref (.+?)>(.+?)<hi (.+?)>(.+?)<\/hi><\/ref>(.+?)<\/xr>/is' => '<div class="related_articles"><a $2>$3</a></div><i>$5</i>',
     '/<xr type="ecGRW" (.+?)><ref (.+?)>(.+?)<hi (.+?)>(.+?)<\/hi><\/ref>(.+?)<\/xr>/is' => '<div class="related_articles"><a $2>$3</a></div><i>$5</i>',
    '/<ref (.+?)>(.+?)<\/ref>/is' => '<a $1>$2</a>',
    );
    $replaceTags = array_merge($replaceTags,$dynamicRegex);


    //----------TODO:need to remove this code -------///
    $xml  =  getTextBetweenTags($xml,$replaceTags);
    $xml = preg_replace_callback('/(<cell|<row|<\/cell|<\/row|<item|<\/item|<list|<\/list)/is','tmpConvert', $xml);
    $searchAttributeFromXml = array('target = ','mimetype = ');
    $replaceAttributeFromXml = array('oldtarget = ','oldmimetype = ');
    $xml  =  str_replace($searchAttributeFromXml, $replaceAttributeFromXml, $xml);
    $output  =  $xml;
      $element[$delta]  =  array(
        '#markup'  =>  $output,
      );
    }

    //variable_set('searchTitle','Gran Enciclopèdia Catalana');

    return $element;
  }
}