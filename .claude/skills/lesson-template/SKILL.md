---
name: lesson-template
description: Chapter README.md template with 5-layer structure. Auto-loads when editing chapter READMEs.
globs: ["chapters/*/README.md"]
---

## Required README Structure

# Chapter NN: Title

## Overview
Brief paragraph introducing the concept. Why it matters. What mental model it builds.

## K&R Reference
- **Sections:** §X.Y, §X.Z
- **Exercises:** X-1, X-2, ...
- **Key Quote:** One memorable quote from K&R about this topic

## Layer 1: Concept
What the C11 standard says. Pure language semantics.

## Layer 2: Assembly Translation
What gcc -S -O0 produces. Register allocation, instruction selection.
Include annotated assembly blocks.

## Layer 3: Memory Layout
Where bytes live. Segment (text/data/bss/heap/stack), offsets, sizeof.
Reference the Excalidraw diagrams.

## Layer 4: Diagrams
List of Excalidraw files in diagrams/ directory with descriptions.
![Memory Layout](diagrams/memory-layout.excalidraw)

## Layer 5: Pitfalls & Undefined Behavior
What breaks. Why. How to detect (sanitizers, Valgrind, static analysis).

## Examples
Walk through each example file with explanations.

## Exercises
List exercises with difficulty ratings and K&R references.

## Going Beyond K&R
What K&R couldn't cover: modern tooling, sanitizers, C11 features, security.

## Summary
Key takeaways. What the student should now understand about the machine.
