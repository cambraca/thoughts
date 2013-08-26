<?php

  if (!function_exists('hue_2_rgb')) {
    // Function to convert hue to RGB, called from above
    function hue_2_rgb($v1, $v2, $vh) {
      if ($vh < 0) {
        $vh += 1;
      };

      if ($vh > 1) {
        $vh -= 1;
      };

      if ((6 * $vh) < 1) {
        return ($v1 + ($v2 - $v1) * 6 * $vh);
      };

      if ((2 * $vh) < 1) {
        return ($v2);
      };

      if ((3 * $vh) < 2) {
        return ($v1 + ($v2 - $v1) * ((2 / 3 - $vh) * 6));
      };

      return ($v1);
    }
  }
?><?php

/**
 * @file field.tpl.php
 * Default template implementation to display the value of a field.
 *
 * This file is not used and is here as a starting point for customization only.
 * @see theme_field()
 *
 * Available variables:
 * - $items: An array of field values. Use render() to output them.
 * - $label: The item label.
 * - $label_hidden: Whether the label display is set to 'hidden'.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - field: The current template type, i.e., "theming hook".
 *   - field-name-[field_name]: The current field name. For example, if the
 *     field name is "field_description" it would result in
 *     "field-name-field-description".
 *   - field-type-[field_type]: The current field type. For example, if the
 *     field type is "text" it would result in "field-type-text".
 *   - field-label-[label_display]: The current label position. For example, if
 *     the label position is "above" it would result in "field-label-above".
 *
 * Other variables:
 * - $element['#object']: The entity to which the field is attached.
 * - $element['#view_mode']: View mode, e.g. 'full', 'teaser'...
 * - $element['#field_name']: The field name.
 * - $element['#field_type']: The field type.
 * - $element['#field_language']: The field language.
 * - $element['#field_translatable']: Whether the field is translatable or not.
 * - $element['#label_display']: Position of label display, inline, above, or
 *   hidden.
 * - $field_name_css: The css-compatible field name.
 * - $field_type_css: The css-compatible field type.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_field()
 * @see theme_field()
 *
 * @ingroup themeable
 */

//$category = field_get_items('node', $node, 'field_category');
  if (!$items) {
    $rgbhex = '';
  } else {
    srand($items[0]['#options']['entity']->tid);
    $s = .3; $l = .7; $h = rand(0, 99) / 100;
    
    // Input is HSL value of complementary colour, held in $h2, $s, $l as fractions of 1
    // Output is RGB in normal 255 255 255 format, held in $r, $g, $b
    // Hue is converted using function hue_2_rgb, shown at the end of this code

    if ($s == 0) {
      $r = $l * 255;
      $g = $l * 255;
      $b = $l * 255;
    } else {
      if ($l < 0.5) {
        $var_2 = $l * (1 + $s);
      } else {
        $var_2 = ($l + $s) - ($s * $l);
      };

      $var_1 = 2 * $l - $var_2;
      $r = 255 * hue_2_rgb($var_1, $var_2, $h + (1 / 3));
      $g = 255 * hue_2_rgb($var_1, $var_2, $h);
      $b = 255 * hue_2_rgb($var_1, $var_2, $h - (1 / 3));
    };

    $rhex = sprintf("%02X", round($r));
    $ghex = sprintf("%02X", round($g));
    $bhex = sprintf("%02X", round($b));

    $rgbhex = $rhex . $ghex . $bhex;
  }
?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?> style="background-color: #<?= $rgbhex ?>">
  <?php if (!$label_hidden): ?>
    <div class="field-label"<?php print $title_attributes; ?>><?php print $label ?>:&nbsp;</div>
  <?php endif; ?>
  <div class="field-items"<?php print $content_attributes; ?>>
    <?php foreach ($items as $delta => $item): ?>
      <div class="field-item <?php print $delta % 2 ? 'odd' : 'even'; ?>"<?php print $item_attributes[$delta]; ?>><?php print render($item); ?></div>
    <?php endforeach; ?>
  </div>
</div>

