<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang xml:lang>
<body>
<div class="wrapper">
<header id="title-block-header">
<h1 class="title" style="text-align:center">Generic Directed Weighted Graph</h1>

</header>
<div style="clear:both">
<div id="TOC" role="doc-toc">
<h1 id="toctitle">Contents</h1>
<ul>
<li><a href="#1-change-log"><span class="toc-section-number">1</span> Change Log<span></span></a></li>
<li><a href="#2-the-task"><span class="toc-section-number">2</span> The Task<span></span></a>
<ul>
<li><a href="#21-definitions-gdwgdefinitions"><span class="toc-section-number">2.1</span> Definitions [gdwg.definitions]<span></span></a></li>
<li><a href="#22-constructors-gdwgctor"><span class="toc-section-number">2.2</span> Constructors [gdwg.ctor]<span></span></a></li>
<li><a href="#23-modifiers-gdwgmodifiers"><span class="toc-section-number">2.3</span> Modifiers [gdwg.modifiers]<span></span></a></li>
<li><a href="#24-accessors-gdwgaccessors"><span class="toc-section-number">2.4</span> Accessors [gdwg.accessors]<span></span></a></li>
<li><a href="#25-iterator-access-gdwgiteratoraccess"><span class="toc-section-number">2.5</span> Iterator access [gdwg.iterator.access]<span></span></a></li>
<li><a href="#26-comparisons-gdwgcmp"><span class="toc-section-number">2.6</span> Comparisons [gdwg.cmp]<span></span></a></li>
<li><a href="#27-extractor-gdwgio"><span class="toc-section-number">2.7</span> Extractor [gdwg.io]<span></span></a></li>
<li><a href="#28-iterator-gdwgiterator"><span class="toc-section-number">2.8</span> Iterator [gdwg.iterator]<span></span></a>
<ul>
<li><a href="#281-iterator-constructor-gdwgiteratorctor"><span class="toc-section-number">2.8.1</span> Iterator constructor [gdwg.iterator.ctor]<span></span></a></li>
<li><a href="#282-iterator-source-gdwgiteratorsource"><span class="toc-section-number">2.8.2</span> Iterator source [gdwg.iterator.source]<span></span></a></li>
<li><a href="#283-iterator-traversal-gdwgiteratortraversal"><span class="toc-section-number">2.8.3</span> Iterator traversal [gdwg.iterator.traversal]<span></span></a></li>
<li><a href="#284-iterator-comparison-gdwgiteratorcomparison"><span class="toc-section-number">2.8.4</span> Iterator comparison [gdwg.iterator.comparison]<span></span></a></li>
</ul></li>
<li><a href="#29-compulsory-internal-representation-gdwginternal"><span class="toc-section-number">2.9</span> Compulsory internal representation [gdwg.internal]<span></span></a>
<ul>
<li><a href="#291-but-why-smart-pointers-gdwginternalrationale"><span class="toc-section-number">2.9.1</span> But why smart pointers [gdwg.internal.rationale]<span></span></a></li>
</ul></li>
<li><a href="#210-other-notes-othernotes"><span class="toc-section-number">2.10</span> Other notes [other.notes]<span></span></a>
<ul>
<li><a href="#2101-const-correctness-constcorrectness"><span class="toc-section-number">2.10.1</span> <code class="sourceCode default">const</code>-correctness [const.correctness]<span></span></a></li>
<li><a href="#2102-member-types-gdwgtypes"><span class="toc-section-number">2.10.2</span> Member types [gdwg.types]</a></li>
</ul></li>
</ul></li>
</div>
<h1 data-number="1" id="change-log"><span class="header-section-number">1</span> Change Log<a href="#change-log" class="self-link"></a></h1>

- **2022-07-26**: Clarified <code> operator-></code> and printing empty graphs (see <a href="https://edstem.org/au/courses/8629/discussion/937184"> operator-></a> and <a href="https://edstem.org/au/courses/8629/discussion/940172?comment=2111014"> printing empty graphs</a>)
- **2022-07-18**: Fixed a typo in <code> insert_edge </code> (see <a href="https://edstem.org/au/courses/8629/discussion/935897"> this post for details</a>)
- **2022-07-14**: Clarified gdwg.internal (see <a href="https://edstem.org/au/courses/8629/discussion/933929"> this post for details</a>)
- **2022-07-11**: Initial Release

<h1 data-number="2" id="the-task"><span class="header-section-number">2</span> The Task<a href="#the-task" class="self-link"></a></h1>
<p>Write a <code class="sourceCode default">graph</code> library type in C++, in <code class="sourceCode default">include/gdwg/graph.hpp</code>.</p>
<p>In this assignment, you will write a <em>generic directed weighted graph</em> (GDWG) with value-semantics in C++. Both the data stored at a node and the weight stored at an edge will be parameterised types. The types may be different. For example, here is a graph with nodes storing <code class="sourceCode default">std::string</code> and edges weighted by <code class="sourceCode default">int</code>:</p>
<div class="sourceCode" id="cb1"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb1-1"><a href="#cb1-1" aria-hidden="true"></a><span class="kw">using</span> graph <span class="op">=</span> gdwg<span class="op">::</span>graph<span class="op">&lt;</span>std<span class="op">::</span>string, <span class="dt">int</span><span class="op">&gt;</span>;</span></code></pre></div>
<p>Formally, this directed weighted graph <span class="math inline"><em>G</em> = (<em>N</em>, <em>E</em>)</span> will consist of a set of nodes <span class="math inline"><em>N</em></span> and a set of weighted edges <span class="math inline"><em>E</em></span>.</p>
<p>All nodes are unique, that is to say, no two nodes will have the same value and shall not compare equal using <code class="sourceCode default">operator==</code>.</p>
<p>Given a node, an edge directed into it is called an <em>incoming edge</em> and an edge directed out of it is called an <em>outgoing edge</em>. The <em>in-degree</em> of a node is the number of its incoming edges. Similarly, the <em>out-degree</em> of a node is the number of its outgoing edges. Given a directed edge from <code class="sourceCode default">src</code> to <code class="sourceCode default">dst</code>, <code class="sourceCode default">src</code> is the <em>source node</em> and <code class="sourceCode default">dst</code> is known as the <em>destination node</em>.</p>
<p>Edges can be reflexive, that is to say, the source and destination nodes of an edge could be the same.</p>
<p><span class="math inline"><em>G</em></span> is a multi-edged graph, as there may be two edges from the same source node to the same destination node with two different weights. Two edges from the same source node to the same destination node cannot have the same weight.</p>
<h2 data-number="2.1" id="definitions-gdwg.definitions"><span class="header-section-number">2.1</span> Definitions [gdwg.definitions]<a href="#definitions-gdwg.definitions" class="self-link"></a></h2>
<ol type="1">
<li><p>Some words have special meaning in this document. This section precisicely defines those words.</p>
<ul>
<li><em>Preconditions</em>: the conditions that the function assumes to hold whenever it is called; violation of any preconditions results in undefined</li>
<li><em>Effects</em>: the actions performed by the function.</li>
<li><em>Postconditions</em>: the conditions (sometimes termed observable results) established by the function.</li>
<li><em>Returns</em>: a description of the value(s) returned by the function.</li>
<li><em>Throws</em>: any exceptions thrown by the function, and the conditions that would cause the exception.</li>
<li><em>Complexity</em>: the time and/or space complexity of the function.</li>
<li><em>Remarks</em>: additional semantic constraints on the function.</li>
<li><em>Unspecified</em>: the implementation is allowed to make its own decisions regarding what is unspecified, provided that it still follows the explicitly specified wording.</li>
</ul></li>
<li><p>An <em>Effects</em> element may specify semantics for a function <code class="sourceCode default">F</code> in code using the term <em>Equivalent to</em>. The semantics for <code class="sourceCode default">F</code> are interpreted as follows:</p>
<ul>
<li>All of the above terminology applies to the provided code, whether or not it is explicitly specified. [<em>Example</em>: If <code class="sourceCode default">F</code> has a <em>Preconditions</em> element, but the code block doesn’t explicitly check them, then it is implied that the preconditions have been checked. —<em>end example</em>]</li>
<li>If there is not a <em>Returns</em> element, and <code class="sourceCode default">F</code> has a non-<code class="sourceCode default">void</code> return type, all the return statements are in the code block.</li>
<li><em>Throws</em>, <em>Postconditions</em>, and <em>Complexity</em> elements always have priority over the code block.</li>
</ul></li>
<li><p>Specified complexity requirements are upper bounds, and implementations that provide better complexity guarantees meet the requirements.</p></li>
<li><p>The class synopsis is the minimum text your header requires to compile most tests (this doesn’t mean that it will necessarily link or run as expected).</p></li>
<li><p>Blue text in code will link to C++ Reference or to another part of this document.</p></li>
<li><p>This section makes use of [stable.names]. A stable name is a short name for a (sub)section, and isn’t supposed to change. We will use these to reference specific sections of the document. [<em>Example</em>:</p></li>
</ol>
<blockquote>
<p>Student: Do we need to define <code class="sourceCode default">gdwg::graph&lt;N, E&gt;::operator!=</code>?</p>
<p>Tutor: [other.notes] mentions that you don’t need to so you can get used to C++20’s generated operators.</p>
</blockquote>
<p>—<em>end example</em>]</p>
<h2 data-number="2.2" id="constructors-gdwg.ctor"><span class="header-section-number">2.2</span> Constructors [gdwg.ctor]<a href="#constructors-gdwg.ctor" class="self-link"></a></h2>
<p><strong>It’s very important your constructors work. If we can’t validly construct your objects, we can’t test any of your other functions.</strong></p>
<div class="sourceCode" id="cb2"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb2-1"><a href="#cb2-1" aria-hidden="true"></a>graph<span class="op">()</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Effects</em>: <a href="https://en.cppreference.com/w/cpp/language/value_initialization">Value initialises</a> all members.</p></li>
<li><p><em>Throws</em>: Nothing.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb3"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb3-1"><a href="#cb3-1" aria-hidden="true"></a>graph<span class="op">(</span>std<span class="op">::</span>initializer_list<span class="op">&lt;</span>N<span class="op">&gt;</span> il<span class="op">)</span>;</span></code></pre></div>
<ol start="3" type="1">
<li><em>Effects</em>: Equivalent to: <code class="sourceCode default">graph(il.begin(), il.end());</code></li>
</ol>
<p><br /></p>

<div class="sourceCode" id="cb4"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb4-1"><a href="#cb4-1" aria-hidden="true"></a><span class="kw">template</span><span class="op">&lt;</span>typename InputIt<span class="op">&gt;</span>
<span id="cb4-3"><a href="#cb4-3" aria-hidden="true"></a>graph<span class="op">(</span>InputIt first, InputIt last<span class="op">)</span>;</span></code></pre></div>
<ol start="4" type="1">
<li><em>Preconditions</em>: Type <code>InputIt</code> models <em><a href="https://en.cppreference.com/w/cpp/named_req/InputIterator">Cpp17InputIterator</a></em> and is indirectly readable as type <code>N</code>.</li>
<li><em>Effects</em>: Initialises the graph’s node collection with the range <code class="sourceCode default">[first, last)</code>.</li>
</ol>
<p><br /></p>

<div class="sourceCode" id="cb6"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb6-1"><a href="#cb6-1" aria-hidden="true"></a>graph<span class="op">(</span>graph<span class="op">&amp;&amp;</span> other<span class="op">)</span> <span class="kw">noexcept</span>;</span></code></pre></div>
<ol start="6" type="1">
<li><em>Postconditions</em>:</li>
</ol>
<ul>
<li><code class="sourceCode default">*this</code> is equal to the value <code class="sourceCode default">other</code> had before this constructor’s invocation.</li>
<li><code class="sourceCode default">other.empty()</code> is <code class="sourceCode default">true</code>.</li>
<li>All iterators pointing to elements owned by <code class="sourceCode default">*this</code> prior to this constructor’s invocation are invalidated.</li>
<li>All iterators pointing to elements owned by <code class="sourceCode default">other</code> prior to this constructor’s invocation remain valid, but now point to the elements owned by <code class="sourceCode default">*this</code>.</li>
</ul>
<p><br /></p>
<div class="sourceCode" id="cb7"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb7-1"><a href="#cb7-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">=(</span>graph<span class="op">&amp;&amp;</span> other<span class="op">)</span> <span class="kw">noexcept</span> <span class="op">-&gt;</span> graph<span class="op">&amp;</span>;</span></code></pre></div>
<ol start="7" type="1">
<li><em>Effects</em>: All existing nodes and edges are either move-assigned to, or are destroyed.</li>
<li><em>Postconditions</em>:</li>
</ol>
<ul>
<li><code class="sourceCode default">*this</code> is equal to the value <code class="sourceCode default">other</code> had before this operator’s invocation.</li>
<li><code class="sourceCode default">other.empty()</code> is <code class="sourceCode default">true</code>.</li>
<li>All iterators pointing to elements owned by <code class="sourceCode default">*this</code> prior to this operator’s invocation are invalidated.</li>
<li>All iterators pointing to elements owned by <code class="sourceCode default">other</code> prior to this operator’s invocation remain valid, but now point to the elements owned by <code class="sourceCode default">*this</code>.</li>
</ul>
<ol start="9" type="1">
<li><em>Returns</em>: <code class="sourceCode default">*this</code>.</li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb8"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb8-1"><a href="#cb8-1" aria-hidden="true"></a>graph<span class="op">(</span>graph <span class="kw">const</span><span class="op">&amp;</span> other<span class="op">)</span>;</span></code></pre></div>
<ol start="10" type="1">
<li><em>Postconditions</em>: <code class="sourceCode default">*this == other</code> is <code class="sourceCode default">true</code>.</li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb9"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb9-1"><a href="#cb9-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">=(</span>graph <span class="kw">const</span><span class="op">&amp;</span> other<span class="op">)</span> <span class="op">-&gt;</span> graph<span class="op">&amp;</span>;</span></code></pre></div>
<ol start="11" type="1">
<li><p><em>Postconditions</em>:</p>
<ul>
<li><code class="sourceCode default">*this == other</code> is <code class="sourceCode default">true</code>.</li>
<li>All iterators pointing to elements owned by <code class="sourceCode default">*this</code> prior to this operator’s invocation are invalidated.</li>
</ul></li>
<li><p><em>Returns</em>: <code class="sourceCode default">*this</code>.</p></li>
</ol>
<h2 data-number="2.3" id="modifiers-gdwg.modifiers"><span class="header-section-number">2.3</span> Modifiers [gdwg.modifiers]<a href="#modifiers-gdwg.modifiers" class="self-link"></a></h2>
<div class="sourceCode" id="cb10"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb10-1"><a href="#cb10-1" aria-hidden="true"></a><span class="kw">auto</span> insert_node<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> value<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Effects</em>: Adds a new node with value <code class="sourceCode default">value</code> to the graph if, and only if, there is no node equivalent to <code class="sourceCode default">value</code> already stored.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if the node is added to the graph and <code class="sourceCode default">false</code> otherwise.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb11"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb11-1"><a href="#cb11-1" aria-hidden="true"></a><span class="kw">auto</span> insert_edge<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src, N <span class="kw">const</span><span class="op">&amp;</span> dst, E <span class="kw">const</span><span class="op">&amp;</span> weight<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="4" type="1">
<li><p><em>Effects</em>: Adds a new edge representing <code class="sourceCode default">src</code> → <code class="sourceCode default">dst</code> with weight <code class="sourceCode default">weight</code>, if, and only if, there is no edge equivalent to <code class="sourceCode default">value_type{src, dst, weight}</code> already stored. [<em>Note</em>:⁠ Nodes are allowed to be connected to themselves. —<em>end note</em>]</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if the edge is added to the graph and <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::insert_edge when either src or dst node does not exist&quot;)</code> if either of <code class="sourceCode default">is_node(src)</code> or <code class="sourceCode default">is_node(dst)</code> are <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb12"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb12-1"><a href="#cb12-1" aria-hidden="true"></a><span class="kw">auto</span> replace_node<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> old_data, N <span class="kw">const</span><span class="op">&amp;</span> new_data<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="8" type="1">
<li><p><em>Effects</em>: Replaces the original data, <code class="sourceCode default">old_data</code>, stored at this particular node by the replacement data, <code class="sourceCode default">new_data</code>. Does nothing if <code class="sourceCode default">new_data</code> already exists as a node.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">false</code> if a node that contains value <code class="sourceCode default">new_data</code> already exists and <code class="sourceCode default">true</code> otherwise.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::replace_node on a node that doesn&#39;t exist&quot;)</code> if <code class="sourceCode default">is_node(old_data)</code> is <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb13"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb13-1"><a href="#cb13-1" aria-hidden="true"></a><span class="kw">auto</span> merge_replace_node<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> old_data, N <span class="kw">const</span><span class="op">&amp;</span> new_data<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">void</span>;</span></code></pre></div>
<ol start="12" type="1">
<li><p><em>Effects</em>: The node equivalent to <code class="sourceCode default">old_data</code> in the graph are replaced with instances of <code class="sourceCode default">new_data</code>. After completing, every incoming and outgoing edge of <code class="sourceCode default">old_data</code> becomes an incoming/ougoing edge of <code class="sourceCode default">new_data</code>, except that duplicate edges shall be removed.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::merge_replace_node on old or new data if they don&#39;t exist in the graph&quot;)</code> if either of <code class="sourceCode default">is_node(old_data)</code> or <code class="sourceCode default">is_node(new_data)</code> are <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
<li><p>[<em>Note</em>: The following examples use the format <span class="math inline">(<em>N</em><sub><em>s</em><em>r</em><em>c</em></sub>, <em>N</em><sub><em>d</em><em>s</em><em>t</em></sub>, <em>E</em>)</span>. [<em>Example</em>: Basic example.</p></li>
</ol>
<ul>
<li>Operation: <code class="sourceCode default">merge_replace_node(A, B)</code></li>
<li>Graph before: <span class="math inline">(<em>A</em>, <em>B</em>, 1), (<em>A</em>, <em>C</em>, 2), (<em>A</em>, <em>D</em>, 3)</span></li>
<li>Graph after : <span class="math inline">(<em>B</em>, <em>B</em>, 1), (<em>B</em>, <em>C</em>, 2), (<em>B</em>, <em>D</em>, 3)</span></li>
</ul>
<p>—<em>end example</em>][<em>Example</em>: Duplicate edge removed example.</p>
<ul>
<li>Operation: <code class="sourceCode default">merge_replace_node(A, B)</code></li>
<li>Graph before: <span class="math inline">(<em>A</em>, <em>B</em>, 1), (<em>A</em>, <em>C</em>, 2), (<em>A</em>, <em>D</em>, 3), (<em>B</em>, <em>B</em>, 1)</span></li>
<li>Graph after : <span class="math inline">(<em>B</em>, <em>B</em>, 1), (<em>B</em>, <em>C</em>, 2), (<em>B</em>, <em>D</em>, 3)</span></li>
</ul>
<p>—<em>end example</em>][<em>Example</em>: Diagrammatic example.</p>
<p><img src="https://i.imgur.com/gCDHqrD.png" /></p>
<p>—<em>end example</em>] —<em>end note</em>]</p>
<p><br /></p>
<div class="sourceCode" id="cb14"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb14-1"><a href="#cb14-1" aria-hidden="true"></a><span class="kw">auto</span> erase_node<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> value<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="16" type="1">
<li><p><em>Effects</em>: Erases all nodes equivalent to <code class="sourceCode default">value</code>, including all incoming and outgoing edges.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if <code class="sourceCode default">value</code> was removed; <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb15"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb15-1"><a href="#cb15-1" aria-hidden="true"></a><span class="kw">auto</span> erase_edge<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src, N <span class="kw">const</span><span class="op">&amp;</span> dst, E <span class="kw">const</span><span class="op">&amp;</span> weight<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="20" type="1">
<li><p><em>Effects</em>: Erases an edge representing <code class="sourceCode default">src</code> → <code class="sourceCode default">dst</code> with weight <code class="sourceCode default">weight</code>.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if an edge was removed; <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::erase_edge on src or dst if they don&#39;t exist in the graph&quot;)</code> if either <code class="sourceCode default">is_node(src)</code> or <code class="sourceCode default">is_node(dst)</code> is <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(log (<em>n</em>) + <em>e</em>)</span>, where <span class="math inline"><em>n</em></span> is the total number of stored nodes and <span class="math inline"><em>e</em></span> is the total number of stored edges.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb16"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb16-1"><a href="#cb16-1" aria-hidden="true"></a><span class="kw">auto</span> erase_edge<span class="op">(</span>iterator i<span class="op">)</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="25" type="1">
<li><p><em>Effects</em>: Erases the edge pointed to by <code class="sourceCode default">i</code>.</p></li>
<li><p><em>Complexity</em>: Amortised constant time.</p></li>
<li><p><em>Returns</em>: An iterator pointing to the element immediately after <code class="sourceCode default">i</code> prior to the element being erased. If no such element exists, returns <code class="sourceCode default">end()</code>.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated. [<em>Note</em>: The postcondition is slightly stricter than a real-world container to help make the assingment easier (i.e. we won’t be testing any iterators post-erasure). —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb17"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb17-1"><a href="#cb17-1" aria-hidden="true"></a><span class="kw">auto</span> erase_edge<span class="op">(</span>iterator i, iterator s<span class="op">)</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="29" type="1">
<li><p><em>Effects</em>: Erases all edges between the iterators <code class="sourceCode default">[i, s)</code>.</p></li>
<li><p><em>Complexity</em> <span class="math inline"><em>O</em>(<em>d</em>)</span>, where <span class="math inline"><em>d</em>=</span><code class="sourceCode default">std::distance(i, s)</code>.</p></li>
<li><p><em>Returns</em>: An iterator equivalent to <code class="sourceCode default">s</code> prior to the items iterated through being erased. If no such element exists, returns <code class="sourceCode default">end()</code>.</p></li>
<li><p><em>Postconditions</em>: All iterators are invalidated. [<em>Note</em>: The postcondition is slightly stricter than a real-world container to help make the assingment easier (i.e. we won’t be testing any iterators post-erasure). —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb18"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb18-1"><a href="#cb18-1" aria-hidden="true"></a><span class="kw">auto</span> clear<span class="op">()</span> <span class="kw">noexcept</span> <span class="op">-&gt;</span> <span class="dt">void</span>;</span></code></pre></div>
<ol start="33" type="1">
<li><p><em>Effects</em>: Erases all nodes from the graph.</p></li>
<li><p><em>Postconditions</em>: <code class="sourceCode default">empty()</code> is <code class="sourceCode default">true</code>.</p></li>
</ol>
<h2 data-number="2.4" id="accessors-gdwg.accessors"><span class="header-section-number">2.4</span> Accessors [gdwg.accessors]<a href="#accessors-gdwg.accessors" class="self-link"></a></h2>
<div class="sourceCode" id="cb19"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb19-1"><a href="#cb19-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> is_node<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> value<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if a node equivalent to <code class="sourceCode default">value</code> exists in the graph, and <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(log (<em>n</em>))</span> time.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb20"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb20-1"><a href="#cb20-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> empty<span class="op">()</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="3" type="1">
<li><em>Returns</em>: <code class="sourceCode default">true</code> if there are no nodes in the graph, and <code class="sourceCode default">false</code> otherwise.</li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb21"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb21-1"><a href="#cb21-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> is_connected<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src, N <span class="kw">const</span><span class="op">&amp;</span> dst<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol start="4" type="1">
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if an edge <code class="sourceCode default">src</code> → <code class="sourceCode default">dst</code> exists in the graph, and <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::is_connected if src or dst node don&#39;t exist in the graph&quot;)</code> if either of <code class="sourceCode default">is_node(src)</code> or <code class="sourceCode default">is_node(dst)</code> are <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb22"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb22-1"><a href="#cb22-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> nodes<span class="op">()</span> <span class="op">-&gt;</span> std<span class="op">::</span>vector<span class="op">&lt;</span>N<span class="op">&gt;</span>;</span></code></pre></div>
<ol start="6" type="1">
<li><p><em>Returns</em>: A sequence of all stored nodes, sorted in ascending order.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(<em>n</em>)</span>, where <span class="math inline"><em>n</em></span> is the number of stored nodes.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb23"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb23-1"><a href="#cb23-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> weights<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src, N <span class="kw">const</span><span class="op">&amp;</span> dst<span class="op">)</span> <span class="op">-&gt;</span> std<span class="op">::</span>vector<span class="op">&lt;</span>E<span class="op">&gt;</span>;</span></code></pre></div>
<ol start="8" type="1">
<li><p><em>Returns</em>: A sequence of weights from <code class="sourceCode default">src</code> to <code class="sourceCode default">dst</code>, sorted in ascending order.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(log (<em>n</em>) + <em>e</em>)</span>, where <span class="math inline"><em>n</em></span> is the number of stored nodes and <span class="math inline"><em>e</em></span> is the number of stored edges.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::weights if src or dst node don&#39;t exist in the graph&quot;)</code> if either of <code class="sourceCode default">is_node(src)</code> or <code class="sourceCode default">is_node(dst)</code> are <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb24"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb24-1"><a href="#cb24-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> find<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src, N <span class="kw">const</span><span class="op">&amp;</span> dst, E <span class="kw">const</span><span class="op">&amp;</span> weight<span class="op">)</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="11" type="1">
<li><p><em>Returns</em>: An iterator pointing to an edge equivalent to <code class="sourceCode default">value_type{src, dst, weight}</code>, or <code class="sourceCode default">end()</code> if no such edge exists.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(log (<em>n</em>) + log (<em>e</em>))</span>, where <span class="math inline"><em>n</em></span> is the number of stored nodes and <span class="math inline"><em>e</em></span> is the number of stored edges.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb25"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb25-1"><a href="#cb25-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> connections<span class="op">(</span>N <span class="kw">const</span><span class="op">&amp;</span> src<span class="op">)</span> <span class="op">-&gt;</span> std<span class="op">::</span>vector<span class="op">&lt;</span>N<span class="op">&gt;</span>;</span></code></pre></div>
<ol start="13" type="1">
<li><p><em>Returns</em>: A sequence of nodes (found from any immediate outgoing edge) connected to <code class="sourceCode default">src</code>, sorted in ascending order, with respect to the connected nodes.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(log (<em>n</em>) + <em>e</em>)</span>, where <span class="math inline"><em>e</em></span> is the number of outgoing edges associated with <code class="sourceCode default">src</code>.</p></li>
<li><p><em>Throws</em>: <code class="sourceCode default">std::runtime_error(&quot;Cannot call gdwg::graph&lt;N, E&gt;::connections if src doesn&#39;t exist in the graph&quot;)</code> if <code class="sourceCode default">is_node(src)</code> is <code class="sourceCode default">false</code>. [<em>Note</em>: Unlike Assignment 2, the exception message must be used verbatim. —<em>end note</em>]</p></li>
</ol>
<h2 data-number="2.5" id="iterator-access-gdwg.iterator.access"><span class="header-section-number">2.5</span> Iterator access [gdwg.iterator.access]<a href="#iterator-access-gdwg.iterator.access" class="self-link"></a></h2>
<div class="sourceCode" id="cb26"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb26-1"><a href="#cb26-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> begin<span class="op">()</span> <span class="kw">const</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol type="1">
<li><em>Returns</em>: An iterator pointing to the first element in the container.</li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb27"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb27-1"><a href="#cb27-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> end<span class="op">()</span> <span class="kw">const</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="2" type="1">
<li><p><em>Returns</em>: An iterator denoting the end of the iterable list that <code class="sourceCode default">begin()</code> points to.</p></li>
<li><p><em>Remarks</em>: <code class="sourceCode default">[begin(), end())</code> shall denote a valid iterable list.</p></li>
</ol>
<h2 data-number="2.6" id="comparisons-gdwg.cmp"><span class="header-section-number">2.6</span> Comparisons [gdwg.cmp]<a href="#comparisons-gdwg.cmp" class="self-link"></a></h2>
<div class="sourceCode" id="cb28"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb28-1"><a href="#cb28-1" aria-hidden="true"></a><span class="op">[[</span><span class="at">nodiscard</span><span class="op">]]</span> <span class="kw">auto</span> <span class="kw">operator</span><span class="op">==(</span>graph <span class="kw">const</span><span class="op">&amp;</span> other<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Returns</em>: <code class="sourceCode default">true</code> if <code class="sourceCode default">*this</code> and <code class="sourceCode default">other</code> contain exactly the same nodes and edges, and <code class="sourceCode default">false</code> otherwise.</p></li>
<li><p><em>Complexity</em>: <span class="math inline"><em>O</em>(<em>n</em> + <em>e</em>)</span> where <span class="math inline"><em>n</em></span> is the sum of stored nodes in <code class="sourceCode default">*this</code> and <code class="sourceCode default">other</code>, and <span class="math inline"><em>e</em></span> is the sum of stored edges in <code class="sourceCode default">*this</code> and <code class="sourceCode default">other</code>.</p></li>
</ol>
<h2 data-number="2.7" id="extractor-gdwg.io"><span class="header-section-number">2.7</span> Extractor [gdwg.io]<a href="#extractor-gdwg.io" class="self-link"></a></h2>
<div class="sourceCode" id="cb29"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb29-1"><a href="#cb29-1" aria-hidden="true"></a><span class="kw">friend</span> <span class="kw">auto</span> <span class="kw">operator</span><span class="op">&lt;&lt;(</span>std<span class="op">::</span>ostream<span class="op">&amp;</span> os, graph <span class="kw">const</span><span class="op">&amp;</span> g<span class="op">)</span> <span class="op">-&gt;</span> std<span class="op">::</span>ostream<span class="op">&amp;</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Effects</em>: Behaves as a <a href="https://en.cppreference.com/w/cpp/named_req/FormattedOutputFunction">formatted output function</a> of <code class="sourceCode default">os</code>.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">os</code>.</p></li>
<li><p><em>Remarks</em>: The format is specified thusly:</p></li>
</ol>
<div class="sourceCode" id="cb30"><pre class="sourceCode default"><code class="sourceCode default"><span id="cb30-1"><a href="#cb30-1" aria-hidden="true"></a>[source_node<span class="math inline"><sub>1</sub></span>] [edges<span class="math inline"><sub>1</sub></span>]</span>
<span id="cb30-2"><a href="#cb30-2" aria-hidden="true"></a>[source_node<span class="math inline"><sub>2</sub></span>] [edges<span class="math inline"><sub>2</sub></span>]</span>
<span id="cb30-3"><a href="#cb30-3" aria-hidden="true"></a>...</span>
<span id="cb30-4"><a href="#cb30-4" aria-hidden="true"></a>[source_node<span class="math inline"><sub><em>n</em></sub></span>] [edges<span class="math inline"><sub><em>n</em></sub></span>]</span></code></pre></div>
<p><code class="sourceCode default">[source_node<span class="math inline"><sub>1</sub></span>]</code>, …, <code class="sourceCode default">[source_node<span class="math inline"><sub><em>n</em></sub></span>]</code> are placeholders for all nodes that the graph stores, sorted in ascending order. <code class="sourceCode default">[edges<span class="math inline"><sub>1</sub></span>]</code>, …, <code class="sourceCode default">[edges<span class="math inline"><sub><em>n</em></sub></span>]</code> are placeholders for</p>
<div class="sourceCode" id="cb31"><pre class="sourceCode default"><code class="sourceCode default"><span id="cb31-1"><a href="#cb31-1" aria-hidden="true"></a>(</span>
<span id="cb31-2"><a href="#cb31-2" aria-hidden="true"></a>  [node<span class="math inline"><sub><em>n</em></sub></span>_connected_node<span class="math inline"><sub>1</sub></span>] | [weight]</span>
<span id="cb31-3"><a href="#cb31-3" aria-hidden="true"></a>  [node<span class="math inline"><sub><em>n</em></sub></span>_connected_node<span class="math inline"><sub>2</sub></span>] | [weight]</span>
<span id="cb31-4"><a href="#cb31-4" aria-hidden="true"></a>  ...</span>
<span id="cb31-5"><a href="#cb31-5" aria-hidden="true"></a>  [node<span class="math inline"><sub><em>n</em></sub></span>_connected_node<span class="math inline"><sub><em>n</em></sub></span>] | [weight]</span>
<span id="cb31-6"><a href="#cb31-6" aria-hidden="true"></a>)</span></code></pre></div>
<p>where <code class="sourceCode default">[node<span class="math inline"><sub><em>n</em></sub></span>_conencted_node<span class="math inline"><sub>1</sub></span>] | [weight]</code>, …, <code class="sourceCode default">[node<span class="math inline"><sub><em>n</em></sub></span>_connected_node<span class="math inline"><sub><em>n</em></sub></span>] | [weight]</code> are placeholders for each node’s connections and corresponding weight, also sorted in ascending order. [<em>Note</em>: If a node doesn’t have any connections, then its corresponding <code class="sourceCode default">[edges<span class="math inline"><sub><em>n</em></sub></span>]</code> should be a line-separated pair of parentheses —<em>end note</em>]</p>
<p>[<em>Example</em>:</p>
<div class="sourceCode" id="cb32"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb32-1"><a href="#cb32-1" aria-hidden="true"></a><span class="kw">using</span> graph <span class="op">=</span> gdwg<span class="op">::</span>graph<span class="op">&lt;</span><span class="dt">int</span>, <span class="dt">int</span><span class="op">&gt;</span>;</span>
<span id="cb32-2"><a href="#cb32-2" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">const</span> v <span class="op">=</span> std<span class="op">::</span>vector<span class="op">&lt;</span>graph<span class="op">::</span>value_type<span class="op">&gt;{</span></span>
<span id="cb32-3"><a href="#cb32-3" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">4</span>, <span class="dv">1</span>, <span class="op">-</span><span class="dv">4</span><span class="op">}</span>,</span>
<span id="cb32-4"><a href="#cb32-4" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">3</span>, <span class="dv">2</span>, <span class="dv">2</span><span class="op">}</span>,</span>
<span id="cb32-5"><a href="#cb32-5" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">2</span>, <span class="dv">4</span>, <span class="dv">2</span><span class="op">}</span>,</span>
<span id="cb32-6"><a href="#cb32-6" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">2</span>, <span class="dv">1</span>, <span class="dv">1</span><span class="op">}</span>,</span>
<span id="cb32-7"><a href="#cb32-7" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">6</span>, <span class="dv">2</span>, <span class="dv">5</span><span class="op">}</span>,</span>
<span id="cb32-8"><a href="#cb32-8" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">6</span>, <span class="dv">3</span>, <span class="dv">10</span><span class="op">}</span>,</span>
<span id="cb32-9"><a href="#cb32-9" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">1</span>, <span class="dv">5</span>, <span class="op">-</span><span class="dv">1</span><span class="op">}</span>,</span>
<span id="cb32-10"><a href="#cb32-10" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">3</span>, <span class="dv">6</span>, <span class="op">-</span><span class="dv">8</span><span class="op">}</span>,</span>
<span id="cb32-11"><a href="#cb32-11" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">4</span>, <span class="dv">5</span>, <span class="dv">3</span><span class="op">}</span>,</span>
<span id="cb32-12"><a href="#cb32-12" aria-hidden="true"></a>  <span class="op">{</span><span class="dv">5</span>, <span class="dv">2</span>, <span class="dv">7</span><span class="op">}</span>,</span>
<span id="cb32-13"><a href="#cb32-13" aria-hidden="true"></a><span class="op">}</span>;</span>
<span id="cb32-14"><a href="#cb32-14" aria-hidden="true"></a></span>
<span id="cb32-15"><a href="#cb32-15" aria-hidden="true"></a><span class="kw">auto</span> g <span class="op">=</span> graph<span class="op">{}</span>;</span>
for (const auto& [from, to, weight] : v) {
  g.insert_node(from);
  g.insert_node(to);
  g.insert_edge(from, to, weight)
}
<span></span>
<span id="cb32-16"><a href="#cb32-16" aria-hidden="true"></a>g<span class="op">.</span>insert_node<span class="op">(</span><span class="dv">64</span><span class="op">)</span>;</span>
<span id="cb32-17"><a href="#cb32-17" aria-hidden="true"></a><span class="kw">auto</span> out <span class="op">=</span> std<span class="op">::</span>ostringstream<span class="op">{}</span>;</span>
<span id="cb32-18"><a href="#cb32-18" aria-hidden="true"></a>out <span class="op">&lt;&lt;</span> g;</span>
<span id="cb32-19"><a href="#cb32-19" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">const</span> expected_output <span class="op">=</span> std<span class="op">::</span>string_view<span class="op">(</span><span class="st">R&quot;(1 (</span></span>
<span id="cb32-20"><a href="#cb32-20" aria-hidden="true"></a><span class="st">  5 | -1</span></span>
<span id="cb32-21"><a href="#cb32-21" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-22"><a href="#cb32-22" aria-hidden="true"></a><span class="st">2 (</span></span>
<span id="cb32-23"><a href="#cb32-23" aria-hidden="true"></a><span class="st">  1 | 1</span></span>
<span id="cb32-24"><a href="#cb32-24" aria-hidden="true"></a><span class="st">  4 | 2</span></span>
<span id="cb32-25"><a href="#cb32-25" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-26"><a href="#cb32-26" aria-hidden="true"></a><span class="st">3 (</span></span>
<span id="cb32-27"><a href="#cb32-27" aria-hidden="true"></a><span class="st">  2 | 2</span></span>
<span id="cb32-28"><a href="#cb32-28" aria-hidden="true"></a><span class="st">  6 | -8</span></span>
<span id="cb32-29"><a href="#cb32-29" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-30"><a href="#cb32-30" aria-hidden="true"></a><span class="st">4 (</span></span>
<span id="cb32-31"><a href="#cb32-31" aria-hidden="true"></a><span class="st">  1 | -4</span></span>
<span id="cb32-32"><a href="#cb32-32" aria-hidden="true"></a><span class="st">  5 | 3</span></span>
<span id="cb32-33"><a href="#cb32-33" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-34"><a href="#cb32-34" aria-hidden="true"></a><span class="st">5 (</span></span>
<span id="cb32-35"><a href="#cb32-35" aria-hidden="true"></a><span class="st">  2 | 7</span></span>
<span id="cb32-36"><a href="#cb32-36" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-37"><a href="#cb32-37" aria-hidden="true"></a><span class="st">6 (</span></span>
<span id="cb32-38"><a href="#cb32-38" aria-hidden="true"></a><span class="st">  2 | 5</span></span>
<span id="cb32-39"><a href="#cb32-39" aria-hidden="true"></a><span class="st">  3 | 10</span></span>
<span id="cb32-40"><a href="#cb32-40" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-41"><a href="#cb32-41" aria-hidden="true"></a><span class="st">64 (</span></span>
<span id="cb32-42"><a href="#cb32-42" aria-hidden="true"></a><span class="st">)</span></span>
<span id="cb32-43"><a href="#cb32-43" aria-hidden="true"></a><span class="st">)&quot;</span><span class="op">)</span>;</span>
<span id="cb32-44"><a href="#cb32-44" aria-hidden="true"></a>CHECK<span class="op">(</span>out<span class="op">.</span>str<span class="op">()</span> <span class="op">==</span> expected_output<span class="op">)</span>;</span></code></pre></div>
<p>—<em>end example</em> ]</p>
<strong> Note: </strong> The empty graph should print an empty string. i.e.

<code> auto g = graph<int, int>(); <br />
auto oss = std::ostringstream{}; <br />
oss << g; <br />
CHECK(oss.str().empty());
</code>

<h2 data-number="2.8" id="iterator-gdwg.iterator"><span class="header-section-number">2.8</span> Iterator [gdwg.iterator]<a href="#iterator-gdwg.iterator" class="self-link"></a></h2>
<div class="sourceCode" id="cb33"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb33-1"><a href="#cb33-1" aria-hidden="true"></a><span class="kw">template</span><span class="op">&lt;</span>typename N, typename E<span class="op">&gt;</span></span>
<span id="cb33-3"><a href="#cb33-3" aria-hidden="true"></a><span class="kw">class</span> graph<span class="op">&lt;</span>N, E<span class="op">&gt;::</span>iterator <span class="op">{</span></span>
<span id="cb33-4"><a href="#cb33-4" aria-hidden="true"></a><span class="kw">public</span><span class="op">:</span></span>
<span id="cb33-5"><a href="#cb33-5" aria-hidden="true"></a>  <span class="kw">using</span> value_type <span class="op">=</span> graph&lt;N, E&gt;::value_type;</span>
<span id="cb33-5"><a href="#cb33-5" aria-hidden="true"></a>  <span class="kw">using</span> reference <span class="op">=</span> value_type;</span>
<span id="cb33-X"><a href="#cb33-X" aria-hidden="true"></a>  <span class="kw">using</span> pointer <span class="op">=</span> <span class="kw">void</span>;</span>
<span id="cb33-6"><a href="#cb33-6" aria-hidden="true"></a>  <span class="kw">using</span> difference_type <span class="op">=</span> std<span class="op">::</span><span class="dt">ptrdiff_t</span>;</span>
<span id="cb33-7"><a href="#cb33-7" aria-hidden="true"></a>  <span class="kw">using</span> iterator_category <span class="op">=</span> std<span class="op">::</span>bidirectional_iterator_tag;</span>
<span id="cb33-8"><a href="#cb33-8" aria-hidden="true"></a></span>
<span id="cb33-9"><a href="#cb33-9" aria-hidden="true"></a>  <span class="co">// Iterator constructor</span></span>
<span id="cb33-10"><a href="#cb33-10" aria-hidden="true"></a>  iterator<span class="op">()</span> <span class="op">=</span> <span class="cf">default</span>;</span>
<span id="cb33-11"><a href="#cb33-11" aria-hidden="true"></a></span>
<span id="cb33-12"><a href="#cb33-12" aria-hidden="true"></a>  <span class="co">// Iterator source</span></span>
<span id="cb33-13"><a href="#cb33-13" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">*()</span> <span class="op">-&gt;</span> reference;</span>
<code>  // auto operator->() -> pointer not required</code>
<span id="cb33-14"><a href="#cb33-14" aria-hidden="true"></a></span>
<span id="cb33-15"><a href="#cb33-15" aria-hidden="true"></a>  <span class="co">// Iterator traversal</span></span>
<span id="cb33-16"><a href="#cb33-16" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">++()</span> <span class="op">-&gt;</span> iterator<span class="op">&amp;</span>;</span>
<span id="cb33-17"><a href="#cb33-17" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">++(</span><span class="dt">int</span><span class="op">)</span> <span class="op">-&gt;</span> iterator;</span>
<span id="cb33-18"><a href="#cb33-18" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">--()</span> <span class="op">-&gt;</span> iterator<span class="op">&amp;</span>;</span>
<span id="cb33-19"><a href="#cb33-19" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">--(</span><span class="dt">int</span><span class="op">)</span> <span class="op">-&gt;</span> iterator;</span>
<span id="cb33-20"><a href="#cb33-20" aria-hidden="true"></a></span>
<span id="cb33-21"><a href="#cb33-21" aria-hidden="true"></a>  <span class="co">// Iterator comparison</span></span>
<span id="cb33-22"><a href="#cb33-22" aria-hidden="true"></a>  <span class="kw">auto</span> <span class="kw">operator</span><span class="op">==(</span>iterator <span class="kw">const</span><span class="op">&amp;</span> other<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span>
<span id="cb33-23"><a href="#cb33-23" aria-hidden="true"></a><span class="kw">private</span><span class="op">:</span></span>
<span id="cb33-24"><a href="#cb33-24" aria-hidden="true"></a>  <span class="kw">explicit</span> iterator<span class="op">(</span><em>unspecified</em><span class="op">)</span>;</span>
<span id="cb33-25"><a href="#cb33-25" aria-hidden="true"></a><span class="op">}</span>;</span></code></pre></div>
<ol type="1">
<li><p>Elements are lexicographically ordered by their source node, destination node, and edge weight, in ascending order.</p></li>
<li><p>Nodes without any connections are not traversed.</p></li>
<li><p>[<em>Note</em>: <code class="sourceCode default">gdwg::graph&lt;N, E&gt;::iterator</code> models <a href="https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator"><code class="sourceCode default">std::bidirectional_iterator</code></a>. —<em>end note</em>]</p></li>
</ol>
<h3 data-number="2.8.1" id="iterator-constructor-gdwg.iterator.ctor"><span class="header-section-number">2.8.1</span> Iterator constructor [gdwg.iterator.ctor]<a href="#iterator-constructor-gdwg.iterator.ctor" class="self-link"></a></h3>
<div class="sourceCode" id="cb34"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb34-1"><a href="#cb34-1" aria-hidden="true"></a>iterator<span class="op">()</span>;</span></code></pre></div>
<ol type="1">
<li><p><em>Effects</em>: Value-initialises all members.</p></li>
<li><p><em>Remarks</em>: Pursuant to the requirements of <a href="https://en.cppreference.com/w/cpp/iterator/forward_iterator"><code class="sourceCode default">std::forward_iterator</code></a>, two value-initialised iterators shall compare equal.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb35"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb35-1"><a href="#cb35-1" aria-hidden="true"></a><span class="kw">explicit</span> iterator<span class="op">(</span><em>unspecified</em><span class="op">)</span>;</span></code></pre></div>
<ol start="3" type="1">
<li><p><em>Effects</em>: Constructs an iterator to a specific element in the graph.</p></li>
<li><p><em>Remarks</em>: There may be multiple constructors with a non-zero number of parameters.</p></li>
</ol>
<h3 data-number="2.8.2" id="iterator-source-gdwg.iterator.source"><span class="header-section-number">2.8.2</span> Iterator source [gdwg.iterator.source]<a href="#iterator-source-gdwg.iterator.source" class="self-link"></a></h3>
<div class="sourceCode" id="cb36"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb36-1"><a href="#cb36-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">*()</span> <span class="op">-&gt;</span> reference;</span></code></pre></div>
<ol type="1">
<li><em>Effects</em>: Returns the current <code>from</code>, <code>to</code>, and <code>weight</code>.</li>
</ol>
<h3 data-number="2.8.3" id="iterator-traversal-gdwg.iterator.traversal"><span class="header-section-number">2.8.3</span> Iterator traversal [gdwg.iterator.traversal]<a href="#iterator-traversal-gdwg.iterator.traversal" class="self-link"></a></h3>
<div class="sourceCode" id="cb38"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb38-1"><a href="#cb38-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">++()</span> <span class="op">-&gt;</span> iterator<span class="op">&amp;</span>;</span></code></pre></div>
<ol type="1">
<li><em>Effects</em>: Advances <code class="sourceCode default">*this</code> to the next element in the iterable list.</li>
</ol>
<p>[<em>Example</em>: In this way, your iterator will iterator through a graph like the one below producing the following tuple values when deferenced each time:</p>
<div class="sourceCode" id="cb39"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb39-1"><a href="#cb39-1" aria-hidden="true"></a>gdwg<span class="op">::</span>graph<span class="op">&lt;</span><span class="dt">int</span>, <span class="dt">int</span><span class="op">&gt;</span></span></code></pre></div>
<p><img src="https://qph.fs.quoracdn.net/main-qimg-2ea8bf9286505bf2ccd63893e05eb5f9" /></p>
<div class="sourceCode" id="cb1"><pre class="sourceCode txt"><code class="sourceCode default"><span id="cb1-1"><a href="#cb1-1" aria-hidden="true"></a>(1, 7, 4)</span>
<span id="cb1-2"><a href="#cb1-2" aria-hidden="true"></a>(1, 12, 3)</span>
<span id="cb1-3"><a href="#cb1-3" aria-hidden="true"></a>(1, 21, 12)</span>
<span id="cb1-4"><a href="#cb1-4" aria-hidden="true"></a>(7, 21, 13)</span>
<span id="cb1-5"><a href="#cb1-5" aria-hidden="true"></a>(12, 19, 16)</span>
<span id="cb1-6"><a href="#cb1-6" aria-hidden="true"></a>(14, 14, 0)</span>
<span id="cb1-7"><a href="#cb1-7" aria-hidden="true"></a>(19, 1, 3)</span>
<span id="cb1-8"><a href="#cb1-8" aria-hidden="true"></a>(19, 21, 2)</span>
<span id="cb1-9"><a href="#cb1-9" aria-hidden="true"></a>(21, 14, 23)</span>
<span id="cb1-10"><a href="#cb1-10" aria-hidden="true"></a>(21, 31, 14)</span></code></pre></div>
<p>—<em>end example</em>]</p>
<ol start="2" type="1">
<li><em>Returns</em>: <code class="sourceCode default">*this</code>.</li>
</ol>
<div class="sourceCode" id="cb40"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb40-1"><a href="#cb40-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">++(</span><span class="dt">int</span><span class="op">)</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="3" type="1">
<li><em>Effects</em>: Equivalent to:</li>
</ol>
<div class="sourceCode" id="cb41"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb41-1"><a href="#cb41-1" aria-hidden="true"></a><span class="kw">auto</span> temp <span class="op">=</span> <span class="op">*</span><span class="kw">this</span>;</span>
<span id="cb41-2"><a href="#cb41-2" aria-hidden="true"></a><span class="op">++*</span><span class="kw">this</span>;</span>
<span id="cb41-3"><a href="#cb41-3" aria-hidden="true"></a><span class="cf">return</span> temp;</span></code></pre></div>
<p><br /></p>
<div class="sourceCode" id="cb42"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb42-1"><a href="#cb42-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">--()</span> <span class="op">-&gt;</span> iterator<span class="op">&amp;</span>;</span></code></pre></div>
<ol start="4" type="1">
<li><p><em>Effects</em>: Advances <code class="sourceCode default">*this</code> to the previous element in the iterable list.</p></li>
<li><p><em>Returns</em>: <code class="sourceCode default">*this</code>.</p></li>
</ol>
<p><br /></p>
<div class="sourceCode" id="cb43"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb43-1"><a href="#cb43-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">--(</span><span class="dt">int</span><span class="op">)</span> <span class="op">-&gt;</span> iterator;</span></code></pre></div>
<ol start="6" type="1">
<li><em>Effects</em>: Equivalent to:</li>
</ol>
<div class="sourceCode" id="cb44"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb44-1"><a href="#cb44-1" aria-hidden="true"></a><span class="kw">auto</span> temp <span class="op">=</span> <span class="op">*</span><span class="kw">this</span>;</span>
<span id="cb44-2"><a href="#cb44-2" aria-hidden="true"></a><span class="op">--*</span><span class="kw">this</span>;</span>
<span id="cb44-3"><a href="#cb44-3" aria-hidden="true"></a><span class="cf">return</span> temp;</span></code></pre></div>
<h3 data-number="2.8.4" id="iterator-comparison-gdwg.iterator.comparison"><span class="header-section-number">2.8.4</span> Iterator comparison [gdwg.iterator.comparison]<a href="#iterator-comparison-gdwg.iterator.comparison" class="self-link"></a></h3>
<div class="sourceCode" id="cb45"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb45-1"><a href="#cb45-1" aria-hidden="true"></a><span class="kw">auto</span> <span class="kw">operator</span><span class="op">==(</span>iterator <span class="kw">const</span><span class="op">&amp;</span> other<span class="op">)</span> <span class="op">-&gt;</span> <span class="dt">bool</span>;</span></code></pre></div>
<ol type="1">
<li><em>Returns</em>: <code class="sourceCode default">true</code> if <code class="sourceCode default">*this</code> and <code class="sourceCode default">other</code> are pointing to the same elements in the same iterable list, and <code class="sourceCode default">false</code> otherwise.</li>
</ol>
<h2 data-number="2.9" id="compulsory-internal-representation-gdwg.internal"><span class="header-section-number">2.9</span> Compulsory internal representation [gdwg.internal]<a href="#compulsory-internal-representation-gdwg.internal" class="self-link"></a></h2>
<p>Your graph is <strong>required</strong> to own the resources (nodes and edge weights) that are passed in through the insert functions. This means creating memory on the heap and doing a proper copy of the values from the caller. This is because resources in your graph should outlive the caller’s resouce that was passed in in case it goes out of scope. For example, we want the following code to be valid.</p>
<div class="sourceCode" id="cb46"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb46-1"><a href="#cb46-1" aria-hidden="true"></a><span class="kw">auto</span> main<span class="op">()</span> <span class="op">-&gt;</span> <span class="dt">int</span> <span class="op">{</span></span>
<span id="cb46-2"><a href="#cb46-2" aria-hidden="true"></a>  gdwg<span class="op">::</span>graph<span class="op">&lt;</span>std<span class="op">::</span>string, <span class="dt">int</span><span class="op">&gt;</span> g;</span>
<span id="cb46-3"><a href="#cb46-3" aria-hidden="true"></a>  <span class="op">{</span></span>
<span id="cb46-4"><a href="#cb46-4" aria-hidden="true"></a>    std<span class="op">::</span>string s1<span class="op">{</span><span class="st">&quot;Hello&quot;</span><span class="op">}</span>;</span>
<span id="cb46-5"><a href="#cb46-5" aria-hidden="true"></a>    g<span class="op">.</span>insert_node<span class="op">(</span>s1<span class="op">)</span>;</span>
<span id="cb46-6"><a href="#cb46-6" aria-hidden="true"></a>  <span class="op">}</span></span>
<span id="cb46-7"><a href="#cb46-7" aria-hidden="true"></a></span>
<span id="cb46-8"><a href="#cb46-8" aria-hidden="true"></a>  <span class="co">// Even though s1 has gone out of scope, g has its own</span></span>
<span id="cb46-9"><a href="#cb46-9" aria-hidden="true"></a>  <span class="co">//  copied resource that it has stored, so the node</span></span>
<span id="cb46-10"><a href="#cb46-10" aria-hidden="true"></a>  <span class="co">//  will still be in here.</span></span>
<span id="cb46-11"><a href="#cb46-11" aria-hidden="true"></a>  std<span class="op">::</span>cout <span class="op">&lt;&lt;</span> g<span class="op">.</span>is_node<span class="op">(</span><span class="st">&quot;Hello&quot;</span><span class="op">)</span> <span class="op">&lt;&lt;</span> <span class="st">&quot;</span><span class="sc">\n</span><span class="st">&quot;</span>; <span class="co">// prints &#39;true&#39;;</span></span>
<span id="cb46-12"><a href="#cb46-12" aria-hidden="true"></a><span class="op">}</span></span></code></pre></div>
<p>Your graph is <strong>required</strong> to use smart pointers (<i>however</i> you please) to solve this problem.</p>
<ol type="1">
<li><p>For each node, you are only allowed to have one underlying resource (heap) stored in your graph for it. This means every <code> N</code> can only be stored once per graph instance.</p></li>
<li><p>For each edge, you should avoid using unnecessary additional memory wherever possible.</p></li>

<li><p>[<em>Hint</em>: In your own implementation you’re likely to use some containers to store things, and depending on your implementation choice, somewhere in those containers you’ll likely use either <code class="sourceCode default">std::unique_ptr&lt;N&gt;</code> or <code class="sourceCode default">std::shared_ptr&lt;N&gt;</code> —<em>end hint</em>]</p></li>
</ol>
<h3 data-number="2.9.1" id="but-why-smart-pointers-gdwg.internal.rationale"><span class="header-section-number">2.9.1</span> But why smart pointers [gdwg.internal.rationale]<a href="#but-why-smart-pointers-gdwg.internal.rationale" class="self-link"></a></h3>
<p>You could feasibly implement the assignment without any smart pointers, through lots of redundant copying. For example, having a massive data structure like:</p>
<div class="sourceCode" id="cb47"><pre class="sourceCode cpp"><code class="sourceCode cpp"><span id="cb47-1"><a href="#cb47-1" aria-hidden="true"></a>std<span class="op">::</span>map<span class="op">&lt;</span>N, std<span class="op">::</span>vector<span class="op">&lt;</span>std<span class="op">::</span>pair<span class="op">&lt;</span>N, E<span class="op">&gt;&gt;&gt;</span></span></code></pre></div>
<p>You can see that in this structure you would have duplicates of nodes when trying to represent this complex structure. This takes up a lot of space. We want you to build a space efficient graph.</p>
<h2 data-number="2.10" id="other-notes-other.notes"><span class="header-section-number">2.10</span> Other notes [other.notes]<a href="#other-notes-other.notes" class="self-link"></a></h2>
<p>You must:</p>
<ul>
<li>Include a header guard in <code class="sourceCode default">include/gdwg/graph.hpp</code></li>
<li>Use C++20 style and methods where appropriate</li>
<li>Make sure that <em>all appropriate member functions</em> are <code class="sourceCode default">const</code>-qualified</li>
<li>Leave a moved-from object in a state with no nodes.</li>
<li>Implement this class within the namespace <code class="sourceCode default">gdwg</code>.</li>
<li>Assignment 2 asked you to implement <code class="sourceCode default">operator!=</code> because you’ll see it in a lot of production codebases, and it’s important that you know how to write it correctly. To get used to how C++20 handles <code class="sourceCode default">operator!=</code>, we’re asking that you <strong>do not</strong> implement an overload for <code class="sourceCode default">operator!=</code> in Assignment 3.</li>
</ul>
<p>You must not:</p>
<ul>
<li>Write to any files that aren’t provided in the repo (e.g. storing your vector data in an auxilliary file)</li>
<li>Add additional members to the <b style="color: red;">public</b> interface.</li>
</ul>
<p>You:</p>
<ul>
<li>Should try to mark member functions that will not throw exceptions with <code class="sourceCode default">noexcept</code></li>
<li>Are not required to make any member function explicit unless directly asked to in the spec.</li>
</ul>
<h3 data-number="2.10.1" id="const-correctness-const.correctness"><span class="header-section-number">2.10.1</span> <code class="sourceCode default">const</code>-correctness [const.correctness]<a href="#const-correctness-const.correctness" class="self-link"></a></h3>
<p>We have deliberately removed the <code class="sourceCode default">const</code>-qualifiers for most member functions from the specification. <strong>You are required to work out which functions must be <code class="sourceCode default">const</code>-qualified.</strong> You must ensure that each operator and member function appropriately either has:</p>
<ul>
<li>A <code class="sourceCode default">const</code> member function; or</li>
<li>A non-<code class="sourceCode default">const</code> member function; or</li>
<li>Both a <code class="sourceCode default">const</code> and non-const member function</li>
</ul>
<p>Please think carefully about this. The function declarations intentionally do not specify their constness, except for the <code class="sourceCode default">begin()</code> and <code class="sourceCode default">end()</code> member functions. These are <code class="sourceCode default">const</code>-qualified to help you out.</p>
<p>In most cases you will only need a single function in the overload set.</p>
<h3 data-number="2.10.2" id="member-types"><span class="header-section-number">2.10.2</span> Member types [gdwg.types]<a href="#member-types" class="self-link"></a></h3>
<p>For convenience's sake, inside the <code>graph</code> class you will likely have a member type <code>value_type</code>, defined like so:</p>
<div class="sourceCode"><pre class="sourceCode cpp"><code class="sourceCode cpp">class graph {
public:
  struct value_type {
    N from;
    N to;
    E weight;
  };
  // ...
};
</code></pre></div>
<p>As noted in <a href="#29-compulsory-internal-representation-gdwginternal">the compulsory internal representation</a> section, you are unlikely to want to use this directly within your representation. However, it is used by the <code>iterator</code> type, and is good practice to have for a container.</P>
</div>
</div>
</body>
</html>
