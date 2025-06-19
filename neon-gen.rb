
FUNCTION_HEADER_TEMPLATE = <<~C_CODE_HEADER
/**
* <%= func_name %>
* This function represents the recursion formula.
* @param r output 128-bit
* @param a a 128-bit part of the internal state array
* @param b a 128-bit part of the internal state array
* @param d a 128-bit part of the internal state array (I/O)
*/
C_CODE_HEADER

# 関数の本体部分のテンプレート
# VLEN と USE_SHA3 をプレースホルダーとして使用
FUNCTION_BODY_TEMPLATE = <<~C_CODE_BODY
  <% if vlen_val == 1 %>
      uint64x2_t v, w, x, y, z;

      x = vld1q_u64((void*)a);
      v = vld1q_u64((void*)b);
  <% else %>
      uint64x2x<%= vlen_val %>_t v, w, x, y, z;

      x = vld1q_u64_x<%= vlen_val %>((void*)a);
      v = vld1q_u64_x<%= vlen_val %>((void*)b);
  <% end %>
  <% if use_sha3_flag == 0 %>
    <% for i in 0 ... vlen_val %>
      w.val[<%= i %>] = veorq_u64(x.val[<%= i %>], NEON_DSFMT_MSK);
    <% end %>
  <% end %>

  <% for i in 0 ... vlen_val %>
      z.val[<%= i %>] = vshlq_n_u64(x.val[<%= i %>], DSFMT_SL1);
  <% end %>

  <% for i in 0 ... vlen_val %>
      z.val[<%= i %>] = veorq_u64(z.val[<%= i %>], v.val[<%= i %>]);
  <% end %>

  <% for i in 0 ... vlen_val-1 %>
      v.val[<%= i %>] = word_reverse(z.val[<%= i %>]);
  <% end %>

      y.val[0] = word_reverse(lung->vqi64);
  <% if vlen_val > 1 %>
      y.val[1] = lung->vqi64;
  <% end %>

  <% if use_sha3_flag == 1 %>
      y.val[0] = veorq_u64(y.val[0], z.val[0]);
    <% if vlen_val > 1 %>
      y.val[1] = veor3q_u64(y.val[1], z.val[1], v.val[0]);
    <% end %>
    <% for i in 2 ... vlen_val %>
      y.val[<%= i %>] = veor3q_u64(y.val[<%= i-2 %>], z.val[<%= i %>], v.val[<%= i-1 %>]);
    <% end %>
  <% else %>
      y.val[0] = veorq_u64(y.val[0], z.val[0]);
    <% if vlen_val > 1 %>
      y.val[1] = veorq_u64(y.val[1], veorq_u64(z.val[1], v.val[0]));
    <% end %>
    <% for i in 2 ... vlen_val %>
      y.val[<%= i %>] = veorq_u64(y.val[<%= i-2 %>], veorq_u64(z.val[<%= i %>], v.val[<%= i-1 %>]));
    <% end %>
  <% end %>

      lung->vqi64 = y.val[<%= vlen_val-1 %>];

  <% for i in 0 ... vlen_val %>
      v.val[<%=i%>] = vshrq_n_u64(y.val[<%=i%>], DSFMT_SR);
  <% end %>
  
  <% if use_sha3_flag == 1 %>
    <% for i in 0 ... vlen_val %>
      w.val[<%= i %>] = vbcaxq_u64(x.val[<%= i %>], y.val[<%= i %>], NEON_DSFMT_MSK_NEG);
    <% end %>
  <% else %>
    <% for i in 0 ... vlen_val %>
      w.val[<%= i %>] = vbslq_u64(y.val[<%= i %>], w.val[<%= i %>], x.val[<%= i %>]);
    <% end %>
  <% end %>

  <% for i in 0 ... vlen_val %>
      v.val[<%= i %>] = veorq_u64(v.val[<%= i %>], w.val[<%= i %>]);
  <% end %>

      vst1q_u64_x<%= vlen_val %>( (void*)r, v);
C_CODE_BODY

require 'erb' # Embedded Ruby

# ----------------------------------------------------
# Cコード生成ロジック
# ----------------------------------------------------

# 各VLENとUSE_SHA3_FLAGの組み合わせで関数を生成
[0, 1].each do |use_sha3_flag|
  [1, 2, 3, 4].each do |vlen_val|
    # 関数名の生成
    func_name = "do_recursion#{vlen_val == 1? '' : '_x' + vlen_val.to_s}#{use_sha3_flag == 1 ? '_sha3' : '_neon'}"

    header = ERB.new(FUNCTION_HEADER_TEMPLATE, nil, '<>')
    puts header.result(binding)

    # 関数の宣言部分
    puts "inline static void #{func_name}(w128_t *r, w128_t *a, w128_t *b, w128_t *lung) "
    puts "{"

    # ERBテンプレートをレンダリングして関数本体を生成
    # ここで vlen_val と use_sha3_flag をテンプレートに渡す
    renderer = ERB.new(FUNCTION_BODY_TEMPLATE, nil, '<>')
    renderer.result(binding).split(/\r?\n|\r/).map {|line|
        if vlen_val == 1
            line = line.gsub('.val[0]', '') 
            next if line.strip.size == 0 
        else
            next if line.size > 0 and line.strip.size == 0 
        end
        
        puts line
    }
    
    # bindingは現在のスコープの変数をERBに渡す

    puts "} " 
    puts
  end
end
